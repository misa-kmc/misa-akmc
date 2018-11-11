################################
# MPI and OpenMP
################################
if (KMC_OpenMP_ENABLE_FLAG)
    find_package(OpenMP REQUIRED)

    if (OPENMP_FOUND)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
    endif ()
endif ()

if (KMC_MPI_ENABLE_FLAG)
    find_package(MPI REQUIRED)
    MESSAGE(STATUS "MPI_INCLUDE dir:" ${MPI_INCLUDE_PATH})
    MESSAGE(STATUS "MPI_LIBRARIES dir:" ${MPI_LIBRARIES})

    if (MPI_CXX_COMPILE_FLAGS)
        set(COMPILE_FLAGS "${COMPILE_FLAGS} ${MPI_CXX_COMPILE_FLAGS}")
    endif ()

    if (MPI_CXX_LINK_FLAGS)
        set(LINK_FLAGS "${LINK_FLAGS} ${MPI_CXX_LINK_FLAGS}")
    endif ()

    include_directories(${MPI_CXX_INCLUDE_PATH})

    set(KMC_EXTRA_LIBS ${KMC_EXTRA_LIBS} ${MPI_CXX_LIBRARIES}) #add mpi lib
endif ()
##### mpi and openmp end


#include(pkg.dep.cmake)
#set(KMC_VENDOR_PATH ${VENDOR_PATH})
#set(KMC_VENDOR_SRC_PATH ${KMC_VENDOR_PATH}/src)
#set(KMC_VENDOR_INCLUDE_PATH ${KMC_VENDOR_PATH}/include)
#set(KMC_VENDOR_PKG_PATH ${KMC_VENDOR_PATH}/pkg)

################################
###### kiwi framework globally
################################
set(KMC_EXTRA_LIBS fmt kiwi ${KMC_EXTRA_LIBS})
