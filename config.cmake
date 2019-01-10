set(KMC_VERSION "0.1.0")

option(KMC_OpenMP_ENABLE_FLAG "Use OpenMP" OFF) #change this flag to OFF to disable OpenMP
option(KMC_MPI_ENABLE_FLAG "Use MPI library" OFF) #change this flag to false to disable mpi
option(KMC_TEST_BUILD_ENABLE_FLAG "Enable test" ON) # enable test
option(KMC_TEST_MPI_ENABLE_FLAG "Enable MPI in test" ON) # enable mpi in test, its value depends on option MPI_ENABLE_FLAG.
option(KMC_TOOLS_BUILD_ENABLE_FLAG "Enable tools building" ON) # building tools directory.
option(KMC_DEBUG_ENABLE_FLAG "Enable tools building" ON) # enable debug mode building.

if (KMC_DEBUG_ENABLE_FLAG)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
endif ()

## architecture ralated values.
# option(ARCH_SW "Enable sunway athread" OFF) # enable sunway athread if its running on sunway system.

#############
## const ##
#############
set(KMC_LIB_NAME kmc)
set(KMC_EXECUTE_BIN_NAME ${PROJECT_NAME})

# test
set(KMC_UINT_TEST_NAME "misa-kmc-unit-tests")