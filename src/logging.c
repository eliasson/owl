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

#include "logging.h"

#include <stdio.h>
#include <stdarg.h>

#define NORMAL_FONT "\033[0m"
#define BOLD_FONT "\033[0;1m"

int OWL_ACTIVE_LOG_LEVEL = OWL_ERROR;

static const char *const LEVEL_NAME[] = {
    [OWL_TRACE]     = "TRACE",
    [OWL_DEBUG]     = "DEBUG",
    [OWL_INFO]      = "INFO ",
    [OWL_WARNING]   = "WARN ",
    [OWL_ERROR]     = "ERROR"
};

static const char *const LEVEL_COLOR[] = {
    [OWL_TRACE]     = "\033[1;1m",
    [OWL_DEBUG]     = "\033[32;1m",
    [OWL_INFO]      = "\033[32;1m",
    [OWL_WARNING]   = "\033[33;1m",
    [OWL_ERROR]     = "\033[41;1m"
};

void owl_log(int level, const char *file, int line, const char *format, ...) {
    if(level < OWL_ACTIVE_LOG_LEVEL)
        return;

	va_list va;

    printf("%s[%s]%s [%s:%d] ",
        LEVEL_COLOR[level],
        LEVEL_NAME[level],
        NORMAL_FONT,
        file,
        line);

	va_start(va, format);
	vprintf(format, va);
	va_end(va);

    printf(NORMAL_FONT);

    fflush(stdout);
}
