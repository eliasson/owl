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

#include "test_queue.h"

// Standard includes
#include <stdlib.h>

// Test code
#include "testbase.h"

// Owl implementation
#include "../src/queue.h"

static void test_create_queue() {
    struct owl_queue *q = new_queue(10);
    
    if(queue_capacity(q) != 10) {
        FAILURE("Expected capacity 10\n");
    }
    TEST;

    free_queue(q);
    TEST;
}

static void test_empty_queue() {
    struct owl_queue *q = new_queue(10);

    if(pop_from_queue(q) != NULL) {
        FAILURE("Expected NULL when popping empty queue\n");
    }
    TEST;

    if(queue_size(q) != 0) {
        FAILURE("Expected size 0 from empty queue\n");
    }
    TEST;

    free_queue(q);
}

static void test_impossible_queue() {
    struct owl_queue *q = new_queue(0);

    void* item = new_track();
    if(append_to_queue(q, item) != 0) {
        FAILURE("Should not be able to append to zero capacity\n");
    }
    free(item);
    TEST;

    free_queue(q);
}

static void test_small_queue() {
    struct owl_queue *q = new_queue(5);

    void* item1 = new_track();
    void* item2 = new_track();
    void* item3 = new_track();
    void* item4 = new_track();
    void* item5 = new_track();
    void* item6 = new_track();

    if(append_to_queue(q, item1) != 1) {
        FAILURE("Should be able to append to queue\n");
    }
    TEST;

    if(queue_size(q) != 1) {
        FAILURE("Expected queue size to be 1 but size is %d\n", queue_size(q));
    }
    TEST;

    append_to_queue(q, item2);
    append_to_queue(q, item3);
    append_to_queue(q, item4);
    append_to_queue(q, item5);

    if(queue_size(q) != 5) {
        FAILURE("Expected queue size to be 5 but size is %d\n", queue_size(q));
    }
    TEST;

    if(append_to_queue(q, item6) != 0) {
        FAILURE("Should not be able to add 6 items to 5 capacity queue\n");
    }
    TEST;

    void* pop = pop_from_queue(q);
    if(pop != item1) {
        FAILURE("Expected to pop first element in queue\n");
    }
    TEST;

    if(queue_size(q) != 4) {
        FAILURE("Expected size to be 4 since last pop\n");
    }
    TEST;

    pop_from_queue(q);
    pop_from_queue(q);
    pop_from_queue(q);
    pop_from_queue(q);

    if(queue_size(q) != 0) {
        FAILURE("Expected size 0 now that all items should be popped\n");
    }

    free(item6);
    free(item5);
    free(item4);
    free(item3);
    free(item2);
    free(item1);

    free_queue(q);
}

static void test_remove_from_queue() {
    struct owl_queue *q = new_queue(5);

    void* item1 = new_track();
    void* item2 = new_track();
    void* item3 = new_track();

    append_to_queue(q, item1);
    if(queue_size(q) != 1) {
        FAILURE("Expected queue size to be 1 but size is %d\n", queue_size(q));
    }
    TEST;

    if(remove_from_queue(q, item2) != 0) {
        FAILURE("Should not be able to remove invalid item\n");
    }

    if(queue_size(q) != 1) {
        FAILURE("Expected queue size to be 1 but size is %d\n", queue_size(q));
    }
    TEST;

    remove_from_queue(q, item1);
    if(queue_size(q) != 0) {
        FAILURE("Expected queue size to be 0 but size is %d\n", queue_size(q));
    }
    TEST;

    append_to_queue(q, item1);
    append_to_queue(q, item2);
    append_to_queue(q, item3);
    if(queue_size(q) != 3) {
        FAILURE("Expected queue size to be 3 but size is %d\n", queue_size(q));
    }
    TEST;

    remove_from_queue(q, item2);
    if(queue_size(q) != 2) {
        FAILURE("Expected queue size to be 3 but size is %d\n", queue_size(q));
    }
    TEST;

    if(pop_from_queue(q) != item1 && pop_from_queue(q) != item3) {
        FAILURE("pop_from_queue did not keep queue consistent");
    }
    TEST;

    free_queue(q);
}

static void test_big_queue() {
    const int BIG = 10000;
    struct owl_queue *q = new_queue(BIG);
    for(int i = 0; i < BIG; i++) {
       append_to_queue(q, new_track());
    }

    if(queue_size(q) != BIG) {
        FAILURE("Expected queue size to be %d but size is %d\n", BIG, queue_size(q));
    }
    TEST;

    // Cleanup
    for(int i = 0; i < BIG; i++) {
        owl_track *item = pop_from_queue(q);
        free_track(item);
    }

    if(queue_size(q) != 0) {
        FAILURE("Expected queue size to be 0 but size is %d\n", queue_size(q));
    }
    TEST;

    free_queue(q);
}

static void test_iterate_queue() {
    struct owl_queue *q = new_queue(5);

    void* item1 = new_track();
    void* item2 = new_track();
    void* item3 = new_track();
    void* item4 = new_track();

    append_to_queue(q, item1);
    append_to_queue(q, item2);
    append_to_queue(q, item3);
    append_to_queue(q, item4);
    
    if(item_at(q, 0) != item1) {
        FAILURE("Expected first item to be at index 0 in queue!\n");
    }
    TEST;

    if(item_at(q, 2) != item3) {
        FAILURE("Expected third item to be at index 2 in queue!\n");
    }
    TEST;

    if(item_at(q, 3) != item4) {
        FAILURE("Expected last item to be at index 3 in queue!\n");
    }
    TEST;
    
    if(item_at(q, 4) != NULL) {
        FAILURE("Expected index 4 to be out of bounds!\n");
    }
    TEST;

    free_queue(q);
}

static void test_big_queue_iteration() {
    const int BIG = 10000;
    struct owl_queue *q = new_queue(BIG);
    for(int i = 0; i < BIG; i++) {
       append_to_queue(q, new_track());
    }

    if(queue_size(q) != BIG) {
        FAILURE("Expected queue size to be %d but size is %d\n", BIG, queue_size(q));
    }
    TEST;

    for(int i = 0; i < BIG; i++) {
        if(item_at(q, i) == NULL) {
            FAILURE("Expected valid item in big queue!\n");
            break;
        }
    }

    free_queue(q);
}

static void test_can_pop_and_later_add_empty_queue() {
    struct owl_queue *q = new_queue(5);

    void* item1 = new_track();

    pop_from_queue(q);
    append_to_queue(q, item1);

    if(queue_size(q) != 1) {
        FAILURE("Expected queue size to be %d but size is %d\n", 1, queue_size(q));
    }
    TEST;

    free_queue(q);
}

void run_queue_tests() {
    test_create_queue();
    test_empty_queue();
    test_impossible_queue();
    test_small_queue();
    test_remove_from_queue();
    test_big_queue();
    test_iterate_queue();
    test_big_queue_iteration();
    test_can_pop_and_later_add_empty_queue();
}
