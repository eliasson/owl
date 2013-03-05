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

#include "queue.h"

#include <stdlib.h>

#include "logging.h"

struct owl_item {
    struct owl_track *item;
    struct owl_item *next;
};

struct owl_queue {
    int capacity;
    int size;
    struct owl_item *head;
    struct owl_item *tail;
};

//
// Creates a new (empty) track
//
owl_track* new_track() {
    owl_track* track = malloc(sizeof(owl_track));
    track->link = NULL;
    track->json = NULL;
    
    return track;
}

//
// Frees any resources allocated when creating the track.
//
void free_track(owl_track *track) {
    if(track == NULL)
        return;
    
    if(track->link != NULL)
        free(track->link);
    if(track->json != NULL)
        free(track->json);
    free(track);
    track = NULL;
}

owl_queue* new_queue(int capacity) {
    owl_queue *queue = malloc(sizeof(owl_queue));
    queue->capacity = capacity;
    queue->size = 0;
    queue->head = NULL;
    queue->tail = NULL;

    return queue;
}

owl_item* new_item(owl_track *track) {
    owl_item *owl_item = malloc(sizeof(owl_item));
    owl_item->item = track;
    owl_item->next = NULL;

    return owl_item;
}

void free_item(owl_item *item) {
    if(item->item != NULL)
        free_track(item->item);
    free(item);
    item = NULL;
}

void free_queue(owl_queue *queue) {
    owl_item *item = queue->head;    
    while(item != NULL) {
        owl_item *old = item;
        item = item->next;
        free_item(old);
    }
    free(queue);
    queue = NULL;
}

int append_to_queue(owl_queue *queue, owl_track *track) {
    if(queue == NULL) {
        WARN("Trying to add to a NULL queue\n");
        return 0;
    }

    if(track == NULL) {
        WARN("Trying to add NULL item to queue, seems strage!\n");
        return 0;
    }
    
    if(queue->capacity < queue->size + 1) {
        WARN("Cannot add to queue, max capacity is reached\n");
        return 0;
    }

    if(queue->head == NULL && queue->tail == NULL) {
        TRACE("Adding first item to empty queue\n");

        owl_item *queue_item = new_item(track);
        
        queue->head = queue->tail = queue_item;
        queue->size++;
    }
    else if(queue->head == NULL || queue->tail == NULL) {
        ERROR("Queue is broken, tail or head should not be NULL separately\n");
        return 0;
    }
    else {
        owl_item *queue_item = new_item(track);

        queue->tail->next = queue_item;
        queue->tail = queue_item;
        queue->size++;
    }

    return 1;
}

int remove_from_queue(owl_queue *queue, owl_track *track) {
    if(queue == NULL) {
        WARN("Trying to remove from a NULL queue\n");
        return 0;
    }

    if((queue->head == NULL && queue->tail == NULL) || queue->size == 0) {
        TRACE("Trying to remove from empty queue\n");
        return 0;
    }
    else if(queue->head == NULL || queue->tail == NULL) {
        ERROR("Queue is broken, tail or head should not be NULL separately\n");
        return 0;
    }
    else if(queue->size == 1) {
        if(queue->head->item == track) {
            owl_item *current = queue->head;
            queue->head = NULL;
            queue->tail = NULL;
            queue->size--;

            free(current);
        }
    }
    else {
        owl_item *i = queue->head;
        while(i != NULL) {
            // Always look one item ahead
            if(i->next != NULL) {
                if(i->next->item == track) {
                    // Ok, next item is the one we're looking for
                    owl_item *to_remove = i->next;
                    owl_item *next_next = i->next->next; // Might be NULL if last element in queue

                    i->next = next_next;
                    queue->size--;

                    free(to_remove);
                }    
            }

            i = i->next;
        }
    }

    return 0;
}

owl_track* pop_from_queue(owl_queue *queue) {
    if(queue == NULL) {
        WARN("Trying to add to a NULL queue\n");
        return NULL;
    }

    if((queue->head == NULL && queue->tail == NULL) || queue->size == 0) {
        TRACE("Popping from empty queue\n");
        return NULL;
    }
    else if(queue->size == 1) {
        owl_item *current_head = queue->head;
        queue->head = queue->tail = NULL;
        queue->size = 0;

        void* value = current_head->item;
        free(current_head);
        return value;
    }
    else if(queue->head == NULL || queue->tail == NULL) {
        ERROR("Queue is broken, tail or head should not be NULL separately\n");
        return NULL;
    }
    else {
        owl_item *current_head = queue->head;
        owl_item *new_head = current_head->next;
        queue->head = new_head;
        queue->size--;

        void* value = current_head->item;
        free(current_head);
        return value;
    }

    return NULL;
}

owl_track* item_at(owl_queue *queue, const int index) {
    if(index >= queue->size) {
        ERROR("Trying to get item out of bounds (index %d but size is %d)\n", index, queue->size);
        return NULL;
    }

    owl_item *item = queue->head;

    int counter = 0;
    while(item != NULL) {    
        
        if(counter == index)
            return item->item;

        item = item->next;
        counter++;
    }

    return NULL;
}

int queue_size(owl_queue *queue) {
    if(queue == NULL)
        return 0;

    return queue->size;
}

int queue_capacity(owl_queue *queue) {
    if(queue == NULL)
        return 0;

    return queue->capacity;
}
