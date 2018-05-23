#include(CodeCoverage)

SET(GCC_COMPILE_FLAGS "-Wall -pedantic")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COMPILE_FLAGS}")

include_directories(inc)

set(TEST_SRC catch.cpp basic_tests.cpp)

add_executable(vector_tests ${TEST_SRC})

target_link_libraries(vector_tests vector)

target_compile_options(vector_tests PRIVATE -g3 -O0 -coverage)
set_target_properties(vector_tests  PROPERTIES LINK_FLAGS "${LINK_FLAGS} -coverage")