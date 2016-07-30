file(GLOB DISPATCHER_SOURCE dispatcher/*.cpp)
file(GLOB DISPATCHER_HEADER dispatcher/*.hpp)

file(GLOB POLICY_CLASSES_SOURCE policy_classes/*.cpp)
file(GLOB POLICY_CLASSES_HEADER policy_classes/*.hpp)

add_library(srcsaxeventdispatch ${DISPATCHER_SOURCE} ${DISPATCHER_HEADER} ${POLICY_CLASSES_SOURCE} ${POLICY_CLASSES_HEADER})