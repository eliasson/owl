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

#include "string.h"

// Standard includes
#include <stdlib.h>
#include <string.h>

#include "constants.h"
#include "logging.h"

// The default size allocated for an empty string
const int DEFAULT_STRING_SIZE = 10;

owl_string* new_empty_string() {
    owl_string *string = malloc(sizeof(owl_string));
    if(string != NULL) {
        char* buffer = malloc((sizeof(char)) * DEFAULT_STRING_SIZE);
        memset(buffer,'\0', (sizeof(char)) * DEFAULT_STRING_SIZE);

        string->ptr = buffer;
        string->size = strlen(buffer);
        string->capacity = DEFAULT_STRING_SIZE;
    }
    
    return string;
}

owl_string* new_string(const char *data) {
    owl_string *string = malloc(sizeof(owl_string));
    if(string != NULL) {
        char* buffer = malloc((sizeof(char)) * DEFAULT_STRING_SIZE);
        memset(buffer,'\0', (sizeof(char)) * DEFAULT_STRING_SIZE);

        strcpy(buffer, data);

        string->ptr = buffer;
        string->size = strlen(buffer);
        string->capacity = DEFAULT_STRING_SIZE;
    }
    return string;
}

void free_string(owl_string *string) {
    if(string->ptr)
        free(string->ptr);
    free(string);
    string = NULL;
}

int append_to_string(owl_string *string, const char *data) {
    const size_t new_size = string->size + strlen(data);

    if(string->capacity < string->size + strlen(data)) {
        char* new_ptr = realloc(string->ptr, new_size);

        if(new_ptr == NULL) {
            return -1;
        }
        else if(string->ptr == new_ptr) {
            // Old block is still used  just update capacity and size
            string->size = new_size;
            string->capacity = new_size;
            strcat(string->ptr, data);
        }
        else {
            // New block
            string->ptr = new_ptr; // realloc will free old block
            string->size = new_size;
            string->capacity = new_size;
            strcat(string->ptr, data);
        }
    }
    else {
        strcat(string->ptr, data);
        string->size = new_size;
    }

    return 0;
}

int string_length(const owl_string *string) {
    return string->size;
}

int starts_with(const owl_string *string, const char *sub_str) {
    return !strncmp(string->ptr, sub_str, strlen(sub_str));
}
