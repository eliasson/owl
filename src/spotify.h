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

#ifndef OWL_SPOTIFY_H
#define OWL_SPOTIFY_H

// Spotify
#include <libspotify/api.h>

// libevent
#include <event.h>

#include "queue.h"

struct spotify_state {
    // libspotify session
    sp_session *session;
    // The queue of owl_tracks
    struct owl_queue *queue;
};

// Internal representation of a track, this is the object used in the queue
struct owl_track {
    // The Spotify link as a string
    char *link;
    // The JSON representation of this track
    char *json;
};

typedef struct spotify_state spotify_state;
typedef struct owl_track owl_track;

//
// Create the Spotify state initialize its members (queue will be initialied but empty)
//
spotify_state* new_spotify_state();

//
// Free any resources allocaed when constructing the Spotify state
//
void free_spotify_state(spotify_state *state);

//
// Creata a new internal representation of a track. This does not contain any references to
// Spotify objects, only meta-data so it can be resolved to a Spotify track later on.
//
// All members will be initialized to NULL or zero.
owl_track* new_track();

//
// Free any resources allocated when created the track
// 
void free_track(struct owl_track *track);

#endif // OWL_SPOTIFY_H
