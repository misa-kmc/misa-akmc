include(configure.cmake)
set(HEADER_FILES
        algorithms/parallel_interface.h
        algorithms/sl/sublattice.h
        lattice/lattice.h
        lattice/lattices_list.h
        lattice/lattice_types.h
        lattice/normal_lattice_list.h
        utils/random/rand_generators.h
        utils/random/random.h
        utils/macros.h
        type_define.h
        creation.h
        )


set(SOURCE_FILES
        algorithms/parallel_interface.cpp
        algorithms/sl/sublattice.cpp
        lattice/lattice.cpp
        lattice/lattices_list.cpp
        lattice/lattice_types.cpp
        lattice/normal_lattice_list.cpp
        utils/random/rand_generators.cpp
        utils/random/random.cpp
        creation.cpp
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