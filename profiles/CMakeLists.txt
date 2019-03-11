# make binary
add_executable(${KMC_EXECUTE_BIN_NAME}
        m_event_listener.h
        m_event_listener.cpp
        main.cpp
        )

target_link_libraries(${KMC_EXECUTE_BIN_NAME} PRIVATE ${KMC_LIB_NAME} ${EXTRA_LIBS})
