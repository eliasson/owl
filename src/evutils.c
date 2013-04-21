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

#include "evutils.h"

// Standard includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include <event2/buffer.h>
#include <event2/util.h>

#include "logging.h"
#include "utils.h"
#include "string.h"

void get_post_argument(evhttp_request *request, const char *arg) {
    unsigned char *post_data = EVBUFFER_DATA(request->input_buffer);
    TRACE("\t%s\n", post_data);
/*
    owl_string *string = new_string(post_data);
    owl_list *list = split_string(string, '&');

    for(int i = 0; i < list->length(), i++) {
        owl_list_item *item = list->at(i);
    }

*/
    /*
    char *request_line;
    size_t len;
    request_line = evbuffer_readln(request->input_buffer, &len, EVBUFFER_EOL_CRLF);

    TRACE("\t%s\n", request_line);
    while(request_line != NULL) {
        TRACE("\t%s\n", request_line);
        if(strstr(request_line, arg) != NULL) {
            TRACE("Matching POST argument: \n\t%s\n", request_line);
        }
        free(request_line);
        request_line = evbuffer_readln(request->input_buffer, &len, EVBUFFER_EOL_CRLF);
    }
    */
    //
    // Activate this code when upgrading to libevent >= 2.1.1-alpha 
    /*
    size_t arg_lengt = strlen(arg);
    
    // Set search start to beginning of buffer
    struct evbuffer_ptr start_ptr;
    evbuffer_ptr_set(request->input_buffer, &start_ptr, 0, EVBUFFER_PTR_SET);

    // Search for argument
    struct evbuffer_ptr occurance_ptr = evbuffer_search(request->input_buffer, arg, arg_lengt, &start_ptr);
    if(occurance_ptr.pos == -1) {
        TRACE("HTTP POST argument %s was not found\n", arg);
    }
    else {
        struct evbuffer_ptr eol_start_ptr;
        struct evbuffer_ptr eol_end_ptr;
        
        evbuffer_ptr_set(request->input_buffer, &eol_start_ptr, occurance_ptr.pos, EVBUFFER_PTR_SET);
        eol_end_ptr = evbuffer_search_eol(request->input_buffer, &eol_start_ptr, NULL, EVBUFFER_EOL_CRLF);

        if(eol_end_ptr.pos > occurance_ptr.pos) {
            size_t data_length = eol_end_ptr.pos - occurance_ptr.pos;
            char *data = create_string(data_length);
            
            evbuffer_copyout_from(request->input_buffer, occurance_ptr, *data, data_length);
            TRACE("HTTP POST argument %s has value %s", arg, data);
        }
    }
    */
}
