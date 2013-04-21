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

#include "test_utils.h"

// Standard includes
#include <stdlib.h>
#include <string.h>

// Test code
#include "testbase.h"

// Owl implementation
#include "../src/utils.h"

static void test_string_starts_with() {    
    TEST;
    if(string_starts_with("Hello world", "Hello") != 1) {
        FAILURE("Test case failed!\n");
    }

    TEST;
    if(string_starts_with("Hello world", "Hello") != 1) {
        FAILURE("Test case failed!\n");
    }

    TEST;
    if(string_starts_with("  Hello world", "  Hello") != 1) {
        FAILURE("Test case failed!\n");
    }
}

static void test_extract_uri_section() {
    TEST;
    char* tc_one = extract_uri_section(0, "");
    if(tc_one != NULL) {
        FAILURE("Test case failed!\n");
    }
    free(tc_one);

    TEST;
    char* tc_two = extract_uri_section(0, "/");
    if(tc_two != NULL) {
        FAILURE("Test case failed!\n");
    }

    free(tc_two);

    TEST;
    char* tc_three = extract_uri_section(2, "/one");
    if(tc_three != NULL) {
        FAILURE("Test case failed!\n");
    }
    free(tc_three);

    TEST;
    char* tc_four = extract_uri_section(0, "/one/two/three");
    if(strcmp(tc_four, "one") != 0) {
        FAILURE("Test case failed expected 'one' but was '%s'!\n", tc_four);
    }
    free(tc_four);

    TEST;
    char* tc_five = extract_uri_section(1, "/one/two/three");
    if(strcmp(tc_five, "two") != 0) {
        FAILURE("Test case failed!\n");
    }
    free(tc_five);

    TEST;
    char* tc_six = extract_uri_section(2, "/one/two/three");
    if(strcmp(tc_six, "three") != 0) {
        FAILURE("Test case failed!\n");
    }
    free(tc_six);

    TEST;
    char* tc_seven = extract_uri_section(3, "/api/login/username/password");
    if(strcmp(tc_seven, "password") != 0) {
        FAILURE("Test case failed! Expected 'password' got '%s'\n", tc_seven);
    }
    free(tc_seven);
}

void run_utils_tests() {
    test_string_starts_with();
    test_extract_uri_section();
}
