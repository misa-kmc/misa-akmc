set(HEADER_FILES
        abvi/defect/def_types.h
        abvi/defect/defect.hpp
        abvi/defect/itl_list.h
        abvi/defect/vacancy_list.h
        abvi/plugin/event_listener.h
        abvi/rate/bonds/bonds_counter.h
        abvi/rate/bonds/energy_dumbbell.h
        abvi/rate/bonds/pair_bond.hpp
        abvi/rate/itl_rates_solver.h
        abvi/rate/rates_types.h
        abvi/rate/rates_solver.h
        abvi/rate/vacancy_rates_solver.h
        abvi/box.h
        abvi/env.h
        abvi/kmc.h
        abvi/orientation.h
        abvi/recombine.h
        )


set(SOURCE_FILES
        abvi/defect/itl_list.cpp
        abvi/defect/vacancy_list.cpp
        abvi/rate/bonds/bonds_counter.cpp
        abvi/rate/bonds/energy_dumbbell.cpp
        abvi/rate/itl_rates_solver.cpp
        abvi/rate/rates_solver.cpp
        abvi/rate/vacancy_rates_solver.cpp
        abvi/box.cpp
        abvi/env.cpp
        abvi/event.h
        abvi/kmc.cpp
        abvi/orientation.cc
        abvi/recombine.cpp
        )


# set binary path and lib storage path for compiling process.
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

# make lib.
add_library(${KMC_MODEL_LIB_NAME} STATIC ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(
        ${KMC_MODEL_LIB_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_link_libraries(${KMC_MODEL_LIB_NAME}
        ${PKMC_LIB_NAME}
        xoshiro::xoshiro
        kiwi::kiwi
        comm::comm
        )

##########################
##### install files
##########################
# install bin and libs.
install(TARGETS ${KMC_MODEL_LIB_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        )

# install header.
install(DIRECTORY "abvi"
        DESTINATION "include"
        FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp"
        )