# find needed libraries
find_package(LibXml2 REQUIRED)

add_definitions("-std=c++11")

link_directories(/usr/local/lib/)
include_directories(${LIBXML2_INCLUDE_DIR}
                    ${CMAKE_SOURCE_DIR}/srcSAX/src/srcsax
                    ${CMAKE_SOURCE_DIR}/srcSAX/src/cpp 
                    policy_classes
                    src)

add_subdirectory(src)
