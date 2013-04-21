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

#include "utils.h"

// Standard includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "logging.h"

#define CONTENT_TYPE_HTML           "text/html"
#define CONTENT_TYPE_PLAIN          "text/plain"
#define CONTENT_TYPE_JS             "application/javascript"
#define CONTENT_TYPE_CSS            "text/css"

char* create_string(const int size) {
    char* buffer = malloc((sizeof(char)) * size);
    memset(buffer,'\0', (sizeof(char)) * size);
    return buffer;
}

char* copy_string(const char * source) {
    char* copy = create_string(strlen(source));
    strcpy(copy, source);
    return copy;
}

int string_starts_with(const char* str, const char* sub_str) {
    return !strncmp(str, sub_str, strlen(sub_str));
}

char* extract_uri_section(int index, const char* str) {
    char* path = copy_string(str);
    char* token = strtok(path, "/");
    int i = 0;

    while(token != NULL) {
        
        if(i == index)
            goto done;

        token = strtok(NULL, "/");
        i++;
        
        if(token == NULL) {
            WARN("Could not extract section nr %d from uri %s.\n", index, path);
            goto done;
        }
    }

done:
    free(path);
    if(token == NULL)
        return NULL;
    return strndup(token, strlen(token));
}

char* resolve_content_type(const char* file_name) {
    const char* ext = file_extension(file_name);
    TRACE("Extension: %s\n", ext);
    if(strcmp(ext, "html") == 0)
        return CONTENT_TYPE_HTML;
    else if(strcmp(ext, "css") == 0)
        return CONTENT_TYPE_CSS;
    else if(strcmp(ext, "js") == 0)
        return CONTENT_TYPE_JS;

    return CONTENT_TYPE_PLAIN;
}

const char* file_extension(const char* file_name) {
    const char *period = strrchr(file_name, '.');
    if(!period || period == file_name)
        return "";
    return period + 1;
}
