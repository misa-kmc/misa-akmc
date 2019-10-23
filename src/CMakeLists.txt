include(configure.cmake)
set(HEADER_FILES
        algorithms/sl/sublattice.h
        algorithms/lattice_region_packer.h
        algorithms/ring_iterator.h
        algorithms/ring.hpp
        algorithms/selector.h
        lattice/lattice.h
        lattice/lattices_list.h
        lattice/lattice_types.h
        lattice/normal_lattice_list.h
        models/abvi/abvi_model.h
        models/model_adapter.h
        pack/ghost_init_packer.h
        pack/ghost_sync_packer.h
        pack/packer_instance.h
        utils/random/rand_generators.h
        utils/random/random.h
        utils/macros.h
        type_define.h
        creation.h
        simulation.h
        )


set(SOURCE_FILES
        algorithms/sl/comm_dirs.h
        algorithms/sl/sublattice.inl
        algorithms/sl/sublattice.cpp
        algorithms/ring_iterator.inc
        algorithms/ring_iterator.h
        lattice/lattice.cpp
        lattice/lattices_list.cpp
        lattice/lattice_types.cpp
        lattice/normal_lattice_list.cpp
        models/abvi/abvi_model.cpp
        models/model_adapter.cpp
        pack/ghost_init_packer.cpp
        pack/ghost_sync_packer.cpp
        pack/packer_instance.cpp
        utils/random/rand_generators.cpp
        utils/random/random.cpp
        creation.cpp
        simulation.cpp
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
target_link_libraries(${PKMC_LIB_NAME}
        kiwi::kiwi
        xoshiro::xoshiro
        comm::comm
        )

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