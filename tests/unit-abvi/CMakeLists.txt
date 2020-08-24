# store temp file, e.g. files for IO test.
set(KMC_TEST_TEMP_STORAGE_PATH ${CMAKE_CURRENT_BINARY_DIR})

# configure a header file to pass some of the CMake settings to the source code
#configure_file(
#        "${CMAKE_CURRENT_SOURCE_DIR}/test/unit/test_config.h.in"
#        "${CMAKE_CURRENT_SOURCE_DIR}/test/unit/test_config.h"
#)

set(TEST_HEADER_FILES
        )

set(TEST_SOURCE_FILES
        itl_test.cpp
        rate/bonds/bonds_counter_test.cpp
        rate/vac_rates_solver_test.cpp
        defect/itl_rates_test.cpp
        orientation_test.cpp
        event_perform_test.cpp
        )


# set binary path and lib storage path wile compiling process.
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

add_executable(${KMC_MODEL_UINT_TEST_NAME} ${TEST_SOURCE_FILES} ${TEST_HEADER_FILES})

## Extra linking for the project: add pnohs lib
#set(PNOHS_EXTRA_LIBS ${PNOHS_LIB_NAME} ${PNOHS_EXTRA_LIBS})

# Extra linking for the project.
if (TARGET GTest::gtest AND TARGET GTest::gtest_main)
    # googletest is added by `find_package(GTest)`
    target_link_libraries(${KMC_MODEL_UINT_TEST_NAME}
            PRIVATE GTest::gtest GTest::gtest_main ${KMC_MODEL_LIB_NAME} ${KMC_EXTRA_LIBS}
            )
else ()
    # googletest is added by `add_subdirectory`
    target_link_libraries(${KMC_MODEL_UINT_TEST_NAME}
            PRIVATE gtest gtest_main ${KMC_MODEL_LIB_NAME} ${KMC_EXTRA_LIBS}
            )
endif ()

# This is so you can do 'make test' to see all your tests run, instead of
# manually running the executable runUnitTests to see those specific tests.
add_test(NAME kmc-test COMMAND ${KMC_MODEL_UINT_TEST_NAME}) # set  environment variable GTEST_COLOR to yes or --gtest_color=no.


##########################
##### install binary files
##########################
install(TARGETS ${KMC_MODEL_UINT_TEST_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        )
