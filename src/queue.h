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

#ifndef OWL_QUEUE_H
#define OWL_QUEUE_H

// Internal representation of a track, this is the object used in the queue
struct owl_track {
    // The Spotify link as a string
    char *link;
    // The JSON representation of this track
    char *json;
};

typedef struct owl_track owl_track;
typedef struct owl_item owl_item;
typedef struct owl_queue owl_queue;

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

//
// Creates an empty queue. The queue's resources is released using free_queue
// 
// Parameters:
//   capacity - The maximum capacity of the queue.
owl_queue* new_queue(int capacity);

//
// Frees the resources used by the queue
//
void free_queue(owl_queue *queue);

//
// Append the given track at the end of the queue. If the queue reached
// its capacity the track will not be addded (no side-effects).
//
// Parameters:
//   queue - The queue to append to
//   track - The track to append
//
// Return:
//   The number of tracks added sucessfully, zero on failure
int append_to_queue(owl_queue *queue, owl_track *track);

//
// Removes the given track from the given queue.
//
// Parameters:
//   queue - The queue to remove the track from
//   track - The track to remove
//
// Return:
//   The number of tracks removed (1 or 0)
int remove_from_queue(owl_queue *queue, owl_track *track);

//
// Pop the next track from the queue. If the queue is NULL or empty NULL will be returned.
//
// Return:
//   The track popped from queue (or NULL)
owl_track* pop_from_queue(owl_queue *queue);

//
// Get a reference to the track at a given index. The track will still be kept in the queue.
// If the index is out of bounds, or for some reason the track cannot be found NULL is returned.
//
// Parameters:
//   queue - The queue to get the track from
//   index - The track's index in queue (starting from 0)
owl_track* item_at(owl_queue *queue, const int index);

//
// Get the current size of the queue. This is the count of actual
// tracks in the queue, not the capacity.
//
int queue_size(owl_queue *queue);

//
// Get the capacity of the queue (not the actual track count).
//
int queue_capacity(owl_queue *queue);

#endif // OWL_QUEUE_H
