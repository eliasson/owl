//
// Copyright (c) 2013 Markus Eliasson
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#define _XOPEN_SOURCE 700               // POSIX 2008

// Standard includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

// libevent
#include <event.h>
#include <evhttp.h>
#include <event2/util.h>
#include <event2/thread.h>
#include <event2/http.h>

// OS
#include <signal.h>
#include <sys/stat.h>

// Application
#include "settings.h"
#include "logging.h"
#include "utils.h"
#include "spotify.h"
#include "spotify_key.h"
#include "errors.h"

// States in the application state machine
#define OWL_STATE_NOT_STARTED       0x01    // Application is not fully started yet (initial state)
#define OWL_STATE_INITIALIZED       0x02    // Application is initialized, but user is not logged in
#define OWL_STATE_LOGGING_IN        0x03    // Application is trying to login to Spotify
#define OWL_STATE_IDLE              0x04    // User is logged in, but no ongoing activity
#define OWL_STATE_PLAYING           0x05    // User is logged in and playing music
#define OWL_STATE_LOGGING_OUT       0x06    // Application is trying to logout from Spotify
#define OWL_STATE_SHUTTING_DOWN     0x07    // Application is shutting down

// Global data
char *doc_root = "static";                  // Relative path to static pages

//
// owl
//
// Main application implementation is in this file. The application uses libevent to be async
// with multiple callbacks regiestered. Libevent is used both for servering the HTTP artifacts
// making up the application UI, as well as controlling playback using the libspotify code
//
// Naming conventions for:
//
// Any function that ends with _handler is a regiestered callback for some libevent event
// Any function that ends with _action is a regiestered HTTP request action
// Any function ending with _callback is a function that is called back from libspotify
//
//
// Note that most action events require some async code when invoking spotify, the request is
// left dangeling until that callback response is retreive, then a a response is sent back to the
// client. I.e. long polling.
//

//
// Application state
//
// Contains pointers to the spotify session and event channels.
// 
// The application's state' state is the state of the state machine, where as 
// the other states are memory states.
//
struct owl_state {
    // Phase in state-machine
    int state;
    // libevent session
    struct event_base* event_base;
    // Event for receiving HTTP traffic
    struct evhttp* http;
    // Event for handling SIGINT
    struct event* sigint;
    // Event for handling SIGTERM
    struct event* sigterm;
    // Event for handling SIGSEGV
    struct event* sigsegv;
    // Event for application specific async callback
    struct event* app;
    // Event for periodic callback (timebased)
    struct event* timer;
    // Timeout setting for timeout event abovee
    struct timeval next_timeout;
    // Owl's spotify state
    struct spotify_state* spotify_state;
    // The HTTP request to write responses to at callback (may be null and is not owning)
    struct evhttp_request *http_request;
};

// 
// Repond with a successful message, formatted as JSON
//  http_request - The HTTP Request to respond to 
static void respond_success(struct evhttp_request *http_request) {
    TRACE("Respond with success\n");

    struct evbuffer *content_buffer = evbuffer_new();

    evbuffer_add_printf(content_buffer, "{ \"status\" : %d, \"message\" : \"%s\" }", OWL_HTTP_NO_ERROR, "OK");
    evhttp_add_header(evhttp_request_get_output_headers(http_request), "Content-type", "application/json; charset=UTF-8");
    evhttp_send_reply(http_request, HTTP_OK, USER_AGENT, content_buffer);
    
    evbuffer_free(content_buffer);
}

//
// Repond with a error message, formatted as JSON
//  http_request - The HTTP Request to respond to 
//  code - The Owl error code to return 
//  message - Error message to write to response
static void respond_error(struct evhttp_request *http_request, int code, const char* message) {
    TRACE("Respond with error (%d)\n", code);

    struct evbuffer *content_buffer = evbuffer_new();

    evbuffer_add_printf(content_buffer, "{ \"status\" : %d, \"message\" : \"%s\" }" , code, message);
    evhttp_add_header(evhttp_request_get_output_headers(http_request), "Content-type", "application/json; charset=UTF-8");
    evhttp_send_reply(http_request, HTTP_INTERNAL, USER_AGENT, content_buffer);

    evbuffer_free(content_buffer);
}

//
// -- Spotify callbacks ---------------------------------------------------------------------------
//
static void logged_in_callback(sp_session *session, sp_error error) {
    TRACE("logged_in_callback\n");
    struct owl_state* state = sp_session_userdata(session);

    if(error == SP_ERROR_OK) {
        state->state = OWL_STATE_IDLE;
        INFO("Logged in to Spotify!\n");
        respond_success(state->http_request);
    }
    else {
        state->state = OWL_STATE_INITIALIZED;
        ERROR("Failed to login to Spotify: %s\n", sp_error_message(error));

        respond_error(state->http_request, OWL_HTTP_ERROR_LOGIN, sp_error_message(error));
    }
}

static void logged_out_callback(sp_session *session) {
    TRACE("logged_out_callback\n");

    struct owl_state* state = sp_session_userdata(session);

    state->state = OWL_STATE_INITIALIZED;
    INFO("Logged out from Spotify\n");

    respond_success(state->http_request);
}

static void notify_main_thread_callback(sp_session *session) {
    TRACE("notify_main_thread_callback\n");

    struct owl_state* state = sp_session_userdata(session);
    event_active(state->app, 0, 1);
}

//
// -- Spotify stuff -------------------------------------------------------------------------------
//
static int initialize_spotify(struct owl_state* state) {
    TRACE("initialize_spotify\n");

    const sp_session_callbacks session_callbacks = {
        .logged_in = &logged_in_callback,
        .logged_out = &logged_out_callback,
        .notify_main_thread = &notify_main_thread_callback
    };

    const sp_session_config session_config = {
        .api_version = SPOTIFY_API_VERSION,
        .cache_location = ".cache/libspotify/",
        .settings_location = ".config/libspotify/",
        .application_key = g_appkey,
        .application_key_size = g_appkey_size,
        .callbacks = &session_callbacks,
        .user_agent = USER_AGENT,
        .userdata = (void*)state,
    };
    

    const int error = sp_session_create(&session_config, &(state->spotify_state->session));
    if (SP_ERROR_OK != error) {
        ERROR("Failed to create Spotify session: %s\n", sp_error_message(error));
        return -1;
    }
    return OWL_OK;
}

//
// -- Actions -------------------------------------------------------------------------------------
//

//
// Action handler to retrieve application state
//
static void state_action(struct owl_state *state) {
    TRACE("Retrieving state....\n");
    struct evbuffer *content_buffer = evbuffer_new();

    evbuffer_add_printf(content_buffer, "{ \"state\" : %d }", state->state);
    evhttp_add_header(evhttp_request_get_output_headers(state->http_request), "Content-type", "application/json; charset=UTF-8");
    evhttp_send_reply(state->http_request, HTTP_OK, USER_AGENT, content_buffer);
    
    evbuffer_free(content_buffer);
}

//
// Action handler that shutdown the server gracefullly
//
static void shutdown_action(struct owl_state* state) {
    TRACE("Asked to shutdown...\n");
    state->state = OWL_STATE_SHUTTING_DOWN;

    event_del(state->sigint);
    event_active(state->sigint, 0, 1);
}

//
// -- Event handlers ------------------------------------------------------------------------------
//

//
// Async callback for handling timeout events
//
static void timer_handler(evutil_socket_t socket, short what, void *userdata) {
    TRACE("Timout event received!\n");

    struct owl_state* state = userdata;
    
    event_del(state->timer);

    int timeout = 0;
    do {
        const sp_error error = sp_session_process_events(state->spotify_state->session, &timeout);
        if(error != SP_ERROR_OK) {
            WARN("Could not process Spotify event: %s", sp_error_message(error));
            break;
        }
            
    } while (timeout == 0);
    
    state->next_timeout.tv_sec = timeout / 1000;
    state->next_timeout.tv_usec = (timeout % 1000) * 1000;
    
    // Reissue timer event
    evtimer_add(state->timer, &state->next_timeout);
}

//
// Handle (certain) unix signals to exit gracefully
//
static void sigint_handler(evutil_socket_t socket, short what, void *userdata) {
    DEBUG("SIGINT | SIGTERM | SIGSEGV received!\n");
    struct owl_state* state = userdata;
    state->state = OWL_STATE_SHUTTING_DOWN;

    // Cleanup event handlers
    event_del(state->sigint);
    event_del(state->sigterm);
    event_del(state->sigsegv);
    event_base_loopbreak(state->event_base);
}

//
// Function called when a new HTTP request have been recieved.
//
static void http_handler(struct evhttp_request *request, void *userdata) {
    struct owl_state* state = userdata;

    // Setup general response headers
    struct evkeyvalq *headers = evhttp_request_get_output_headers(request);
    evhttp_add_header(headers, "Server", USER_AGENT);

    // Get the requested URI
    const char* uri = evhttp_request_get_uri(request);
    const int http_method = evhttp_request_get_command(request);

    if( http_method != EVHTTP_REQ_GET &&
        http_method != EVHTTP_REQ_PUT &&
        http_method != EVHTTP_REQ_POST) {
        evhttp_send_error(request, 501, "Not Implemented");
        return;
    }

    TRACE("Received HTTP request: %s\n", uri);

    // Keep the request for async usage
    state->http_request = request;

    //
    // Retrieve application state (sync)
    if(string_starts_with(uri, "/api/state")) {
        state_action(state);
    }

    //
    // Shutdown owl application (async)
    else if(string_starts_with(uri, "/api/shutdown")) {
        shutdown_action(state);
    }

    //
    // Serve static file immediately
    else {
        // Create the buffer to retrn as content
        struct evbuffer *content_buffer = evbuffer_new();

        // If not a handler this is a static request
        char *static_file = (char *) malloc(strlen(doc_root) + strlen(uri) + 1);
        stpcpy(stpcpy(static_file, doc_root), uri);

        TRACE("Looking for static file: %s\n", static_file);

        bool file_exists = 1;
        struct stat st;
        if(stat(static_file, &st) == -1 || S_ISDIR(st.st_mode)) {
            file_exists = 0;
            evhttp_send_error(request, HTTP_NOTFOUND, "Not Found");
        }

        if(file_exists) {
            const int file_size = st.st_size;
            FILE *fp = fopen(static_file, "r");
            const char* content_type = resolve_content_type(static_file);

            evbuffer_add_file(content_buffer, fileno(fp), 0, file_size); // will close

            TRACE("Resolving content type for filename: %s to: %s\n", static_file, content_type);
            evhttp_add_header(headers, "Content-Type", content_type);
            evhttp_send_reply(request, HTTP_OK, "OK", content_buffer);
        }
        free(static_file);

        // Send the data
        evhttp_send_reply(request, HTTP_OK, USER_AGENT, content_buffer);

        // Free memrory
        evbuffer_free(content_buffer);
    }
    return;
}

//
// -- Main programme ------------------------------------------------------------------------------
//

extern char *optarg; // declared by getopt

static void usage() {
    fprintf(stderr,
        "usage: owl [-b address] [-p port] [-d level] [-h]\n"
        "\t -b Bind server on this local IP address (127.0.0.1)\n"
        "\t -p Port numer to listen to (8080)\n"
        "\t -r Path to document root for HTML (static)"
        "\t -d Enable logging to stdout for either of [TRACE | DEBUG | INFO | WARN | ERROR] (off)\n"
        "\t -h Displays this message\n");
}

int main(int argc, char **argv) {
    short http_port = 8080;
    char* http_addr = "0.0.0.0";

    int arg;

    while( (arg = getopt(argc, argv, "b:p:r:d:h")) != -1) {
        switch(arg) {
            case 'b':
                http_addr = optarg;
                break;

            case 'p':
                http_port = atoi(optarg);
                break;

            case 'r':
                doc_root = optarg;

            case 'd':
                if(strcmp(optarg, "TRACE") == 0)
                    OWL_ACTIVE_LOG_LEVEL = OWL_TRACE;
                else if(strcmp(optarg, "DEBUG") == 0)
                    OWL_ACTIVE_LOG_LEVEL = OWL_DEBUG;
                else if(strcmp(optarg, "INFO") == 0)
                    OWL_ACTIVE_LOG_LEVEL = OWL_INFO;
                else if(strcmp(optarg, "WARN") == 0)
                    OWL_ACTIVE_LOG_LEVEL = OWL_WARNING;
                else
                    OWL_ACTIVE_LOG_LEVEL = OWL_OFF;
                break;

            case 'h':
            default:
                usage();
                exit(1);
        }
    }
  
    // Initialize application
    int status = 0;
    struct owl_state* state = malloc(sizeof(struct owl_state));
    state->spotify_state = new_spotify_state();

    state->state = OWL_STATE_NOT_STARTED;

    // Setup application handlers
    { 
        state->event_base = event_base_new();

        // Unix signal handlers
        state->sigint = evsignal_new(state->event_base, SIGINT, &sigint_handler, state);
        state->sigterm = evsignal_new(state->event_base, SIGTERM, &sigint_handler, state);
        state->sigsegv = evsignal_new(state->event_base, SIGSEGV, &sigint_handler, state);
        evsignal_add(state->sigint, NULL);
        evsignal_add(state->sigterm, NULL);
        evsignal_add(state->sigsegv, NULL);

        // Periodic callback
        state->timer = evtimer_new(state->event_base, &timer_handler, state);

        // Applications programatic callback
        state->app = evtimer_new(state->event_base, &timer_handler, state);

        // HTTP Server
        state->http = evhttp_new(state->event_base);
        evhttp_set_timeout(state->http, 60);
        evhttp_set_gencb(state->http, &http_handler, state);
        status = evhttp_bind_socket(state->http, http_addr, http_port);
    }

    if(status != 0) {
        ERROR("Could not bind owl server on IP (%s) or port (%d)\n", http_addr, http_port);
        goto shutdown;
    }

    status = initialize_spotify(state);
    if(status != 0) {
        ERROR("Could not initalize Spotify\n");
        goto shutdown;
    }

    state->state = OWL_STATE_INITIALIZED;
    INFO("Owl started, connect to http://%s:%d\n", http_addr, http_port);

    // Kick off event loop
    event_base_dispatch(state->event_base);

shutdown:
    INFO("Owl is shutting down\n");

    event_free(state->sigint);
    event_free(state->sigterm);
    event_free(state->sigsegv);
    event_free(state->timer);
    evhttp_free(state->http);
    event_base_free(state->event_base);

    free_spotify_state(state->spotify_state);
    free(state);

    return status;
}