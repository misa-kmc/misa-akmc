cmake_minimum_required(VERSION 3.0)

project(misa-kmc)

set(CMAKE_CXX_STANDARD 11)

include(config.cmake)
include(dependency.cmake)

add_subdirectory(src)

#if (KMC_TOOLS_BUILD_ENABLE_FLAG)
#    add_subdirectory(tools)
#endif ()
################################
## test configure
################################
if (KMC_TEST_BUILD_ENABLE_FLAG)
    MESSAGE(STATUS "TEST is enabled")
    add_subdirectory(tests)
endif ()
