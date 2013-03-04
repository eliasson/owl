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

#ifndef OWL_ERRORS_H
#define OWL_ERRORS_H

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

#endif // OWL_ERRORS_H