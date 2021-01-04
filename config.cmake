set(KMC_VERSION "0.1.0")

option(KMC_OpenMP_ENABLE_FLAG "Use OpenMP" OFF) #change this flag to OFF to disable OpenMP
option(KMC_MPI_ENABLE_FLAG "Use MPI library" ON) #change this flag to false to disable mpi
option(KMC_TEST_BUILD_ENABLE_FLAG "Enable test" ON) # enable test
option(KMC_TOOLS_BUILD_FLAG "Enable building tools" ON) # enable tools building
option(KMC_TEST_MPI_ENABLE_FLAG "Enable MPI in test" ON) # enable mpi in test, its value depends on option MPI_ENABLE_FLAG.

set(KMC_RAND "MT" CACHE STRING "random number generating algorithm") # random number generating
# options are:
#   LCG: linear congruential
#   MT: mersenne twister
#   STC: subtract with carry
#   xoshiro: http://xoshiro.di.unimi.it
#   REAL: real random number privided by linux OS.

# To perform coverage, use `-fprofile-instr-generate -fcoverage-mapping` in clang,
# and `--coverage` or `-fprofile-arcs -ftest-coverage` in gnu.
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -Wall")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -Wall")

if (CMAKE_BUILD_TYPE MATCHES "^(Debug|DEBUG|debug)$")
    set(KMC_DEBUG_ENABLE_FLAG ON)
endif ()

# features
option(EAM_POT_ENABLE_FLAG "Enable using eam potential to calculate system energy" OFF)

## architecture ralated values.
# option(ARCH_SW "Enable sunway athread" OFF) # enable sunway athread if its running on sunway system.

#############
## const ##
#############
set(KMC_MODEL_LIB_NAME abvi)
set(PKMC_LIB_NAME pkmc)
set(KMC_EXECUTE_BIN_NAME ${PROJECT_NAME})
set(KMC_TOOLS_LIB_NAME kmc-tools)
set(KMC_TOOLS_BIN_NAME tools)

# test
set(KMC_MODEL_UINT_TEST_NAME "abvi-model-unit-tests")
set(PKMC_UINT_TEST_NAME "pkmc-unit-tests")
set(TOOLS_UINT_TEST_NAME "tools-tests")
