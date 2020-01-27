set(TEST_SOURCE_FILES
        particle_test.cpp
        )


# set binary path and lib storage path wile compiling process.
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

add_executable(${TOOLS_UINT_TEST_NAME} ${TEST_SOURCE_FILES})

# Extra linking for the project.
if (TARGET GTest::gtest AND TARGET GTest::gtest_main)
    # googletest is added by `find_package(GTest)`
    target_link_libraries(${TOOLS_UINT_TEST_NAME}
            PRIVATE GTest::gtest GTest::gtest_main kiwi::kiwi ${KMC_TOOLS_LIB_NAME} ${KMC_EXTRA_LIBS}
            )
else ()
    # googletest is added by `add_subdirectory`
    target_link_libraries(${TOOLS_UINT_TEST_NAME}
            PRIVATE gtest gtest_main kiwi::kiwi ${KMC_TOOLS_LIB_NAME} ${KMC_EXTRA_LIBS}
            )
endif ()

# This is so you can do 'make test' to see all your tests run, instead of
# manually running the executable runUnitTests to see those specific tests.
add_test(NAME kmc-tools-test COMMAND ${TOOLS_UINT_TEST_NAME}) # set  environment variable GTEST_COLOR to yes or --gtest_color=no.


##########################
##### install binary files
##########################
install(TARGETS ${TOOLS_UINT_TEST_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        )
