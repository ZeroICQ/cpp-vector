#include(CodeCoverage)
SET(GCC_COMPILE_FLAGS "-Wall -pedantic")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COMPILE_FLAGS}")

set(SRC vector.cpp)

add_library(vector ${SRC})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(vector PRIVATE -g3 -O0 -coverage)
    set_target_properties(vector  PROPERTIES LINK_FLAGS "${LINK_FLAGS} -coverage")
endif()
