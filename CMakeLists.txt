##
#  CMakeLists.txt
#
#  Copyright (C) 2025-2025 srcML, LLC. (www.srcML.org)
#
#  This file is part of the srcDispatch.
#
#  The srcDispatch is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  The srcDispatch is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with the srcDispatch; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

cmake_minimum_required(VERSION 3.14)
project(srcDispatch)

include(CTest)

if(NOT WIN32)
    set(CMAKE_INSTALL_PREFIX "/usr/local/")
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_definitions("-Wall")

add_subdirectory(srcSAX/CMake)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SRCDISPATCH_INCLUDE_DIR ${SRCSAX_INCLUDE_DIR}
                         src/policy_classes
                         src/dispatcher
    CACHE INTERNAL "Include directories for srcDispatch")

set(SRCDISPATCH_LIBRARIES ${SRCSAX_LIBRARIES} CACHE INTERNAL "Libraries for srcDispatch")

# include needed includes
include_directories(${SRCDISPATCH_INCLUDE_DIR})

# Continue to src/test
add_subdirectory(src)
add_subdirectory(test)
