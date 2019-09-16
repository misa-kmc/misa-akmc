set(HEADER_FILES
        defect/def_types.h
        defect/defect.hpp
        defect/itl_list.h
        defect/vacancy_list.h
        plugin/event_listener.h
        rate/bonds/bonds_counter.h
        rate/bonds/energy_dumbbell.h
        rate/bonds/pair_bond.hpp
        rate/itl_rates_solver.h
        rate/rates_types.h
        rate/rates_solver.h
        rate/vacancy_rates_solver.h
        box.h
        counter.h
        env.h
        kmc.h
        orientation.h
        recombine.h
        )


set(SOURCE_FILES
        defect/itl_list.cpp
        defect/vacancy_list.cpp
        rate/bonds/bonds_counter.cpp
        rate/bonds/energy_dumbbell.cpp
        rate/itl_rates_solver.cpp
        rate/rates_solver.cpp
        rate/vacancy_rates_solver.cpp
        box.cpp
        counter.cpp
        env.cpp
        event.h
        kmc.cpp
        orientation.cc
        recombine.cpp
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
target_link_libraries(${KMC_LIB_NAME} ${PKMC_LIB_NAME} xoshiro::xoshiro)

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