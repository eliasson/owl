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

#ifndef OWL_CONSTANTS_H
#define OWL_CONSTANTS_H

// States in the application state machine
#define OWL_STATE_NOT_STARTED               0x01    // Application is not fully started yet (initial state)
#define OWL_STATE_INITIALIZED               0x02    // Application is initialized, but user is not logged in
#define OWL_STATE_LOGGING_IN                0x03    // Application is trying to login to Spotify
#define OWL_STATE_IDLE                      0x04    // User is logged in, but no ongoing activity
#define OWL_STATE_PLAYING                   0x05    // User is logged in and playing music
#define OWL_STATE_LOGGING_OUT               0x06    // Application is trying to logout from Spotify
#define OWL_STATE_SHUTTING_DOWN             0x07    // Application is shutting down

// Client side errors used in the HTTP API
#define OWL_HTTP_NO_ERROR                   ( 000)
#define OWL_HTTP_ERROR_NOT_LOGGED_IN        (-100)
#define OWL_HTTO_ERROR_NO_LOGIN_DETAILS     (-101)
#define OWL_HTTP_ERROR_LOGIN                (-102)
#define OWL_HTTP_ERROR_SEARCHING            (-110)
#define OWL_HTTP_UNSUPPORTED_ARTWORK        (-120)
#define OWL_HTTP_ERROR_ARTWORK              (-130)
#define OWL_HTTP_ERROR_RESOLVING_LINK       (-140)
#define OWL_HTTP_ERROR_QUEUE_FULL           (-150)

#endif // OWL_CONSTANTS_H