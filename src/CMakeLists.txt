include(configure.cmake)
set(HEADER_FILES
        atom.h
        type_define.h
        )


set(SOURCE_FILES
        atom.cpp
        )


# set binary path and lib storage path for compiling process.
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

# make lib.
add_library(${KMC_LIB_NAME} STATIC ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(
        ${KMC_LIB_NAME}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

# make binary
add_executable(${KMC_EXECUTE_BIN_NAME} main.cpp )

target_link_libraries(${KMC_EXECUTE_BIN_NAME} PRIVATE ${KMC_LIB_NAME} ${EXTRA_LIBS})


##########################
##### install files
##########################
# install bin and libs.
install(TARGETS ${KMC_LIB_NAME}
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