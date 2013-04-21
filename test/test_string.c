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

#include "test_string.h"

// Standard includes
#include <stdlib.h>

// Test code
#include "testbase.h"

// Owl implementation
#include "../src/string.h"

static void test_can_create_empty_string() {
    owl_string *string = new_empty_string();
    if(string == NULL) {
        FAILURE("Could not create string\n");
    }
    TEST;

    free_string(string);
}

static void test_can_create_string_from_char() {
    owl_string *string = new_string("test");
    if(string == NULL) {
        FAILURE("Could not create string\n");
    }
    TEST;

    if(string_length(string) != 4) {
        FAILURE("Expected length of 4 not %d\n", string_length(string));
    }
    TEST;

    free_string(string);
}

static void test_can_compare_string_start() {
    owl_string *string = new_string("test");
    if(string == NULL) {
        FAILURE("Could not create string\n");
    }

    if(!starts_with(string, "te")) {
        FAILURE("Expected string to start with \"te\" \n");
    }
    TEST;

    if(starts_with(string, "cd")) {
        FAILURE("Did not expected string to start with \"cd\" \n");
    }
    TEST;

    free_string(string);
}

static void test_can_append_to_string() {
    owl_string *string = new_string("test");
    if(string == NULL) {
        FAILURE("Could not create string\n");
    }

    if((append_to_string(string, " something bigger that the default size, something like this") == -1) ||
       (string_length(string) != 64))
       {
        FAILURE("Expected to be able to append to string (%s)\n", string->ptr);
    }
    TEST;

    free_string(string);
}

void run_string_tests() {
    test_can_create_empty_string();
    test_can_create_string_from_char();
    test_can_compare_string_start();
    test_can_append_to_string();
}