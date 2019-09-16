set(KMC_VERSION "0.1.0")

option(KMC_OpenMP_ENABLE_FLAG "Use OpenMP" OFF) #change this flag to OFF to disable OpenMP
option(KMC_MPI_ENABLE_FLAG "Use MPI library" OFF) #change this flag to false to disable mpi
option(KMC_TEST_BUILD_ENABLE_FLAG "Enable test" ON) # enable test
option(KMC_TEST_MPI_ENABLE_FLAG "Enable MPI in test" ON) # enable mpi in test, its value depends on option MPI_ENABLE_FLAG.
option(KMC_TOOLS_BUILD_ENABLE_FLAG "Enable tools building" ON) # building tools directory.

set(KMC_RAND "MT" CACHE STRING "random number generating algorithm") # random number generating
# options are:
#   LCG: linear congruential
#   MT: mersenne twister
#   STC: subtract with carry
#   xoshiro: http://xoshiro.di.unimi.it
#   REAL: real random number privided by linux OS.

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -Wall --coverage")

if (CMAKE_BUILD_TYPE MATCHES "^(Debug|DEBUG|debug)$")
    set(KMC_DEBUG_ENABLE_FLAG ON)
endif ()

# features
option(EAM_POT_EBNABLE_FLAG "Enable using eam potential to calculate system energy" OFF)

## architecture ralated values.
# option(ARCH_SW "Enable sunway athread" OFF) # enable sunway athread if its running on sunway system.

#############
## const ##
#############
set(KMC_LIB_NAME kmc)
set(PKMC_LIB_NAME pkmc)
set(KMC_EXECUTE_BIN_NAME ${PROJECT_NAME})

# test
set(KMC_UINT_TEST_NAME "misa-kmc-unit-tests")