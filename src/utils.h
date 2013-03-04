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

#ifndef OWL_UTILS_H
#define OWL_UTILS_H

#define _XOPEN_SOURCE 700               // POSIX 2008

//
// Create a new string (and allocating new memory).
//
char* create_string(const int size);

//
// Make a copy of the given string (and allocating new memory).
//
char* copy_string(const char * source);

//
// Return TRUE if the given str starts with the given sub_str
//
int string_starts_with(const char* str, const char* sub_str);

//
// Extract and return the n'th seciont of the given uri. A section is a part
// of the uri, sucha as /<section>/<section>/...
//
char* extract_uri_section(int index, const char* str);

//
// Resolve and return the HTTP Content-Type value for the given filename
//
char* resolve_content_type(const char* file_name);

//
// Return the file extension for the given file name. Does not allocate memory, and is only
// a valid pointer until as long as file_name data is kept on heap
//
const char* file_extension(const char* file_name);

#endif
