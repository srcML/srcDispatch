add_executable(testcallpolicy ../src/srcSAXEventDispatch.hpp TestCallPolicy.cpp)
target_link_libraries(testcallpolicy srcsax_static ${LIBXML2_LIBRARIES})