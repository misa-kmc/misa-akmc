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
        lattice_types_test.cpp
        lattices_list_test.cpp
        lattice_list_status_test.cpp
        itl_test.cpp
        )


# set binary path and lib storage path wile compiling process.
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

add_executable(${KMC_UINT_TEST_NAME} ${TEST_SOURCE_FILES} ${TEST_HEADER_FILES})

## Extra linking for the project: add pnohs lib
#set(PNOHS_EXTRA_LIBS ${PNOHS_LIB_NAME} ${PNOHS_EXTRA_LIBS})

# Extra linking for the project.
target_link_libraries(${KMC_UINT_TEST_NAME}
        PRIVATE GTest::gtest GTest::gtest_main ${KMC_LIB_NAME} ${KMC_EXTRA_LIBS}
        )

# This is so you can do 'make test' to see all your tests run, instead of
# manually running the executable runUnitTests to see those specific tests.
add_test(NAME kmc-test COMMAND ${KMC_UINT_TEST_NAME}) # set  environment variable GTEST_COLOR to yes or --gtest_color=no.


##########################
##### install binary files
##########################
install(TARGETS ${PNOHS_UINT_TEST_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        )
