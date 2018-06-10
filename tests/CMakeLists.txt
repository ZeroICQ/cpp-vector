include(CodeCoverage)

SET(GCC_COMPILE_FLAGS "-Wall -pedantic")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COMPILE_FLAGS}")

get_property(vector_include_dir TARGET vector PROPERTY INTERFACE_INCLUDE_DIRECTORIES)

include_directories(inc ${vector_include_dir})

set(TEST_SRC
        catch.cpp
        vector_tests.cpp
        itertator_tests.cpp
        )

add_executable(vector_tests ${TEST_SRC})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(vector_tests PRIVATE -g3 -O0 -coverage)
    set_target_properties(vector_tests  PROPERTIES LINK_FLAGS "${LINK_FLAGS} -coverage")
endif()

SETUP_TARGET_FOR_COVERAGE(
        NAME coverage                 # New target name
        EXECUTABLE bin/vector_tests   # Executable in PROJECT_BINARY_DIR
        DEPENDENCIES vector_tests
)
