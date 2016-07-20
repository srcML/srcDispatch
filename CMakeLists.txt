file(GLOB SRCSAXHANDLER_SOURCE ../../srcSAX/src/srcsax/*.cpp ../../srcSAX/src/cpp/*.cpp)
file(GLOB SRCSAXHANDLER_INCLUDE ../../srcSAX/src/srcsax/*.h* ../../srcSAX/src/cpp/*.h*)

link_directories(/usr/local/lib/)
include_directories(${LIBXML2_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/srcSAX/src/srcsax ${CMAKE_SOURCE_DIR}/srcSAX/src/cpp ../policy_classes)

add_library(srcsaxeventdispatch ./src/srcSAXEventDispatch.cpp ./src/srcSAXEventDispatch.hpp)