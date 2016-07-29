add_executable(testcallpolicy ../src/srcSAXEventDispatch.hpp TestCallPolicy.cpp)
add_executable(testsignaturepolicy ../src/srcSAXEventDispatch.hpp TestFnSignaturePolicy.cpp)
target_link_libraries(testcallpolicy srcsax_static ${LIBXML2_LIBRARIES})
target_link_libraries(testsignaturepolicy srcsax_static ${LIBXML2_LIBRARIES})