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

typedef struct owl_item owl_item;
typedef struct owl_queue owl_queue;

//
// Creates an empty queue. The queue's resources is released using free_queue
// 
// Parameters:
//   capacity - The maximum capacity of the queue.
struct owl_queue* new_queue(int capacity);

//
// Frees the resources used by the queue
//
void free_queue(struct owl_queue *queue);

//
// Append the given item at the end of the queue. If the queue reached
// its capacity the item will not be addded (no side-effects).
//
// Parameters:
//   queue - The queue to append to
//   item - The item to append
//
// Return:
//   The number of items added sucessfully, zero on failure
int append_to_queue(struct owl_queue *queue, void* item);

//
// Removes the given item from the given queue.
//
// Parameters:
//   queue - The queue to remove the element from
//   item - The item to remove
//
// Return:
//   The number of items removed (1 or 0)
int remove_from_queue(struct owl_queue *queue, void* item);

//
// Pop the next item from the queue. If the queue is NULL or empty NULL will be returned.
//
// Return:
//   The item popped from queue (or NULL)
void* pop_from_queue(struct owl_queue *queue);

//
// Get a reference to the item at a given index. The item will still be kept in the queue.
// If the index is out of bounds, or for some reason it cannot be found NULL is returned.
//
// Parameters:
//   queue - The queue to get item from
//   index - The items index in queue (starting from 0)
void* item_at(struct owl_queue *queue, const int index);

//
// Get the current size of the queue. This is the count of actual
// items in the queue, not the capacity.
//
int queue_size(struct owl_queue *queue);

//
// Get the capacity of the queue (not the actual item count).
//
int queue_capacity(struct owl_queue *queue);

#endif // OWL_QUEUE_H
