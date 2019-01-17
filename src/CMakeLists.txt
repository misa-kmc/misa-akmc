include(configure.cmake)
set(HEADER_FILES
        defect/def_types.h
        defect/defect.hpp
        defect/itl_list.h
        defect/vacancy_list.h
        lattice/lattice.h
        lattice/lattices_list.h
        lattice/lattice_types.h
        lattice/normal_lattice_list.h
        box.h
        direction.h
        macros.h
        rates_updater.h
        type_define.h
        )


set(SOURCE_FILES
        defect/itl_list.cpp
        defect/vacancy_list.cpp
        lattice/lattice.cpp
        lattice/lattices_list.cpp
        lattice/lattice_types.cpp
        lattice/normal_lattice_list.cpp
        box.cpp
        direction.cc
        rates_updater.cpp
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