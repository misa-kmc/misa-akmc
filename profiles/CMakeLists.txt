# make binary
add_executable(${KMC_EXECUTE_BIN_NAME} main.cpp)

target_link_libraries(${KMC_EXECUTE_BIN_NAME} PRIVATE ${KMC_LIB_NAME} ${EXTRA_LIBS})
