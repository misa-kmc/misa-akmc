if (NOT (KMC_MPI_ENABLE_FLAG))
    set(KMC_TEST_MPI_ENABLE_FLAG OFF)  # if we disabled KMC_MPI_ENABLE_FLAG, we cann't also use KMC_TEST_MPI_ENABLE_FLAG.
endif ()
MESSAGE(STATUS "MPI in test: ${KMC_TEST_MPI_ENABLE_FLAG}")

add_subdirectory(unit)
add_subdirectory(unit-abvi)
add_subdirectory(unit-tools)
