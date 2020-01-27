set(TOOLS_SOURCE_FILES
        ans.h
        ans.cpp
        particle.h
        xyz_reader.h
        xyz_reader.cpp
        )

add_library(${KMC_TOOLS_LIB_NAME} STATIC ${TOOLS_SOURCE_FILES})
# todo profile lib(used by converting string to lattice  typ):
target_link_libraries(${KMC_TOOLS_LIB_NAME} ${PKMC_LIB_NAME} profile)
target_include_directories(${KMC_TOOLS_LIB_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
        )

add_executable(${KMC_TOOLS_BIN_NAME} main.cpp)
target_link_libraries(${KMC_TOOLS_BIN_NAME} ${KMC_TOOLS_LIB_NAME})
