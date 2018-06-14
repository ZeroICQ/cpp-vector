#include(CodeCoverage)
SET(GCC_COMPILE_FLAGS "-Wall -pedantic")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COMPILE_FLAGS}")

set(SRC main.cpp vector.h vector_iterator.h)

add_executable(vector ${SRC})

target_include_directories(vector INTERFACE .)

#if(CMAKE_BUILD_TYPE STREQUAL "Debug")
#    target_compile_options(vector PRIVATE -ggdb -O0 -coverage)
#    set_target_properties(vector  PROPERTIES LINK_FLAGS "${LINK_FLAGS} -coverage")
#endif()

target_compile_options(vector PRIVATE -ggdb -O0 -coverage)
set_target_properties(vector  PROPERTIES LINK_FLAGS "${LINK_FLAGS} -coverage")
