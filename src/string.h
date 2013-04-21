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

#ifndef OWL_STRING_H
#define OWL_STRING_H

// Standard includes
#include <stdlib.h>

struct owl_string {
    char *ptr;
    size_t size;
    size_t capacity;
};

typedef struct owl_string owl_string;

//
// Allcates a new string of zero size
//
owl_string* new_empty_string();

//
// Create a new string using the given char array. The string object will take a copy
// of the given string and will not take owership of the argument
//
owl_string* new_string(const char *data);

//
// Return any memory used by the given string
//
void free_string(owl_string *string);

//
// Append the given substring to the given string
//
// Return -1 if failed to append string
int append_to_string(owl_string *string, const char *sub_str);

//
// Get the length of the given string
//
int string_length(const owl_string *string);

//
// Return 1 if the given string starts with the given substring
//
int starts_with(const owl_string *string, const char *sub_str);

#endif