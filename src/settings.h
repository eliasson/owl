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

#ifndef OWL_SETTINGS_H
#define OWL_SETTINGS_H

// Common constants
#define TRUE                                1
#define FALSE                               0
#define OWL_OK                              0x00
#define OWL_ERROR_GENERAL                   0x01
#define OWL_ERROR_NOT_INITIALIZED           0x02
#define OWL_ERROR_NOT_LOGGED_IN             0x03

// Application settings
#define MAX_PENDING_CONNECTIONS             100                 // The max number of pending HTTP request
#define BUFFER_SIZE                         1024                // Client request buffer size in bytes
#define USER_AGENT                          "OWL/0.1"           // HTTP User-Agent header

#endif // OWL_SETTINGS_H