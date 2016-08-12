cmake_minimum_required(VERSION 2.8)
project(srcSAXEventDispatch)

 # find needed libraries
find_package(LibXml2 REQUIRED)

add_definitions("-std=c++11")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

link_directories(/usr/local/lib/)
include_directories(${LIBXML2_INCLUDE_DIR}
                    ${CMAKE_SOURCE_DIR}/srcSAX/src/srcsax
                    ${CMAKE_SOURCE_DIR}/srcSAX/src/cpp
                    ${CMAKE_SOURCE_DIR}/srcSAX/src/windows 
                    src/dispatcher
                    src/policy_classes
                    tests)

add_subdirectory(srcSAX/src)
add_subdirectory(src)
add_subdirectory(tests)