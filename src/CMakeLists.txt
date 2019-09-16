include(configure.cmake)
set(HEADER_FILES
        utils/random/rand_generators.h
        utils/random/random.h
        )


set(SOURCE_FILES
        utils/random/rand_generators.cpp
        utils/random/random.cpp
        )


# set binary path and lib storage path for compiling process.
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

# make lib.
add_library(${PKMC_LIB_NAME} STATIC ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(
        ${PKMC_LIB_NAME}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(${PKMC_LIB_NAME} xoshiro::xoshiro comm::comm)

##########################
##### install files
##########################
# install bin and libs.
install(TARGETS ${PKMC_LIB_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        )

# install header.
install(DIRECTORY "./"
        DESTINATION "include"
        FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp"
        )