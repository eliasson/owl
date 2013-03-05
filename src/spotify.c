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

#include "spotify.h"

#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "settings.h"
#include "queue.h"
#include "constants.h"
#include "logging.h"

// Setup the max string lenght for various of objects
#define QUEUE_CAPACITY              100

spotify_state* new_spotify_state() {
    spotify_state *state = malloc(sizeof(spotify_state));
    state->queue = new_queue(QUEUE_CAPACITY);
    return state;
}

void free_spotify_state(spotify_state *state) {
    // TODO: Must free queue contents!
    free_queue(state->queue);
    if(state->session != NULL)
        sp_session_release(state->session); // Ignore error, what to do..
    free(state);
    state = NULL;
}
