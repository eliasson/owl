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

#ifndef OWL_LOGGING_H
#define OWL_LOGGING_H

#define OWL_TRACE       0x00
#define OWL_DEBUG       0x01
#define OWL_INFO        0x02
#define OWL_WARNING     0x03
#define OWL_ERROR       0x04
#define OWL_OFF         0x05

// This must be set to control the log level
extern int OWL_ACTIVE_LOG_LEVEL;

// Do not use this direct, use the macros instead
void owl_log(int level, const char *file, int line, const char* format, ...);

// Macros exposing the logging API
#define TRACE(format, arg...) \
    owl_log(OWL_TRACE, __FILE__, __LINE__, format, ##arg)

#define DEBUG(format, arg...) \
	owl_log(OWL_DEBUG, __FILE__, __LINE__, format, ##arg)

#define INFO(format, arg...) \
	owl_log(OWL_INFO, __FILE__, __LINE__, format, ##arg)

#define WARN(format, arg...) \
	owl_log(OWL_WARNING, __FILE__, __LINE__, format, ##arg)

#define ERROR(format, arg...) \
	owl_log(OWL_ERROR, __FILE__, __LINE__, format, ##arg)

#endif // OWL_LOGGING_H
