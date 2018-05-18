##
#  srcsax_event_dispatch.cmake
#
#  Copyright (C) 2016-2018 srcML, LLC. (www.srcML.org)
#
#  This file is part of the srcSAXEventDispatch.
#
#  The srcSAXEventDispatch is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  The srcSAXEventDispatch is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with the srcSAXEventDispatch; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

get_filename_component(DISPATCH_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
get_filename_component(DISPATCH_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR} DIRECTORY)

add_subdirectory(${DISPATCH_SOURCE_DIR}/srcSAX/CMake ${DISPATCH_BINARY_DIR}/srcSAX/CMake)

# Compiler options
add_definitions("-std=c++11")

# find needed libraries
find_package(LibXml2 REQUIRED)

set(DISPATCH_INCLUDE_DIR ${SRCSAX_INCLUDE_DIR}
                         ${DISPATCH_SOURCE_DIR}/src/dispatcher
                         ${DISPATCH_SOURCE_DIR}/src/policy_classes
    CACHE INTERNAL "Include directories for srcSAXEventDispatch")

set(DISPATCH_LIBRARIES ${SRCSAX_LIBRARIES} CACHE INTERNAL "Libraries for srcSAXEventDispatch")

# include needed includes
include_directories(${DISPATCH_INCLUDE_DIR})

# Continue to build directory
add_subdirectory(${DISPATCH_SOURCE_DIR}/src ${DISPATCH_BINARY_DIR}/src)
