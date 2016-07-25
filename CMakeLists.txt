link_directories(/usr/local/lib/)
include_directories(${LIBXML2_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/srcSAX/src/srcsax ${CMAKE_SOURCE_DIR}/srcSAX/src/cpp ../policy_classes)

add_subdirectory(src)
