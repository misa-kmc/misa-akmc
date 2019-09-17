# set binary path and lib storage path for compiled binary.
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

# make binary
add_executable(${KMC_EXECUTE_BIN_NAME}
        m_event_listener.h
        m_event_listener.cpp
        main.cpp
        )

# link google perftools
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lprofiler")

target_link_libraries(${KMC_EXECUTE_BIN_NAME} PRIVATE ${KMC_LIB_NAME} ${KMC_EXTRA_LIBS})

set(P_KMC_EXECUTE_BIN_NAME pmisa-kmc)
add_executable(${P_KMC_EXECUTE_BIN_NAME}
        profile.cpp
        )

target_link_libraries(${P_KMC_EXECUTE_BIN_NAME} PRIVATE
        kiwi::kiwi
        ${PKMC_LIB_NAME}
        ${KMC_EXTRA_LIBS})

# install bin and libs
install(TARGETS ${KMC_EXECUTE_BIN_NAME} ${P_KMC_EXECUTE_BIN_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        )