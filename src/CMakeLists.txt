include(configure.cmake)
set(HEADER_FILES
        box.h
        lattice/lattice.h
        lattice/lattices_list.h
        lattice/lattice_types.h
        lattice/pure_lattice_list.h
        macros.h
        rates_updater.h
        rates.h
        type_define.h
        vacancy_list.h
        )


set(SOURCE_FILES
        box.cpp
        lattice/lattice.cpp
        lattice/lattices_list.cpp
        lattice/lattice_types.cpp
        lattice/pure_lattice_list.cpp
        rates_updater.cpp
        rates.cpp
        vacancy_list.cpp
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
add_executable(${KMC_EXECUTE_BIN_NAME} main.cpp)

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