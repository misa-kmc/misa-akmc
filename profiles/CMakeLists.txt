# set binary path and lib storage path for compiled binary.
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

# configure a header file to pass some of the CMake settings to the source code
configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/profile_config.h.in"
        "${CMAKE_CURRENT_SOURCE_DIR}/profile_config.h"
)

set(P_KMC_EXECUTE_BIN_NAME pmisa-kmc)
add_executable(${P_KMC_EXECUTE_BIN_NAME}
        profile.cpp
        pkmc.h
        pkmc.cpp
        )

target_link_libraries(${P_KMC_EXECUTE_BIN_NAME} PRIVATE
        kiwi::kiwi
        ${PKMC_LIB_NAME}
        ${KMC_EXTRA_LIBS})

# install bin and libs
install(TARGETS ${P_KMC_EXECUTE_BIN_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        )
