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

    # Get filename without extension
    get_filename_component(TEST_NAME "${TEST_FILE}" NAME_WE)

    # Get parent directory
    get_filename_component(TEST_DIR "${TEST_FILE}" DIRECTORY)

    # Get directory relative to this CMakeLists.txt
    file(RELATIVE_PATH TEST_REL_DIR
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "${TEST_DIR}"
    )

    # Create a unique CMake target name
    if(TEST_REL_DIR STREQUAL "." OR TEST_REL_DIR STREQUAL "")
        set(TEST_TARGET_NAME "${TEST_NAME}")
        set(TEST_OUTPUT_DIR "${CMAKE_BINARY_DIR}/bin")
    else()
        string(REPLACE "/" "_" TEST_DIR_NAME "${TEST_REL_DIR}")
        set(TEST_TARGET_NAME "${TEST_DIR_NAME}_${TEST_NAME}")
        set(TEST_OUTPUT_DIR "${CMAKE_BINARY_DIR}/bin/${TEST_REL_DIR}")
    endif()

    # Unique CMake target
    add_executable(${TEST_TARGET_NAME} ${TEST_FILE})

    target_link_libraries(
        ${TEST_TARGET_NAME}
        $<TARGET_OBJECTS:srcsax_static>
        $<TARGET_OBJECTS:srcdispatch>
        LibXml2::LibXml2
        Boost::boost
        ${ARGN}
    )

    set_target_properties(
        ${TEST_TARGET_NAME}
        PROPERTIES
        OUTPUT_NAME "${TEST_NAME}"
        RUNTIME_OUTPUT_DIRECTORY "${TEST_OUTPUT_DIR}"
    )

    add_test(
        NAME ${TEST_TARGET_NAME}
        COMMAND $<TARGET_FILE:${TEST_TARGET_NAME}>
    )

endmacro()

add_subdirectory(suite)
