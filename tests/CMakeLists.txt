enable_testing()

if (NOT (KMC_MPI_ENABLE_FLAG))
    set(KMC_TEST_MPI_ENABLE_FLAG OFF)  # if we disabled KMC_MPI_ENABLE_FLAG, we cann't also use KMC_TEST_MPI_ENABLE_FLAG.
endif ()
MESSAGE(STATUS "MPI in test: ${KMC_TEST_MPI_ENABLE_FLAG}")

# Standard linking to gtest stuff.
set(KMC_EXTRA_LIBS gtest gtest_main ${KMC_EXTRA_LIBS})

add_subdirectory(unit)