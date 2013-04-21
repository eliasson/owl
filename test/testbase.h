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

#ifndef OWL_TESTBASE_H
#define OWL_TESTBASE_H

#include "../src/logging.h"

extern int TEST_COUNT;
extern int TEST_FAILURES;

// Use this when implementing a new test, this will increas the test counter
#define TEST \
    TEST_COUNT++;

// Use this when a test failes, increase the failure counter
#define FAILURE(format, arg...) \
     TEST_FAILURES++; ERROR(format, ##arg)

#endif // OWL_TESTBASE_H
