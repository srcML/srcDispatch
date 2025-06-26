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

find_package(LibXml2 REQUIRED)
find_package(Boost REQUIRED)

include_directories(util)

macro(add_unit_test TEST_FILE)

    get_filename_component(TEST_NAME_WITH_EXTENSION ${TEST_FILE} NAME)
    string(FIND ${TEST_NAME_WITH_EXTENSION} "." EXTENSION_BEGIN)
    string(SUBSTRING ${TEST_NAME_WITH_EXTENSION} 0 ${EXTENSION_BEGIN} TEST_NAME)

    add_executable(${TEST_NAME} ${TEST_FILE})
    target_link_libraries(${TEST_NAME} $<TARGET_OBJECTS:srcsax_static> $<TARGET_OBJECTS:srcdispatch> LibXml2::LibXml2 Boost::boost ${ARGN})
    add_test(NAME ${TEST_NAME} COMMAND $<TARGET_FILE:${TEST_NAME}>)
    set_target_properties(${TEST_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

endmacro()

add_subdirectory(suite)
