## generate configure file
if (EAM_POT_EBNABLE_FLAG)
    set(EAM_POT_ENABLED ON)
endif ()

if (KMC_DEBUG_ENABLE_FLAG)
    set(DEBUG_MODE ON)
endif ()

configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/building_config.in"
        "${CMAKE_CURRENT_SOURCE_DIR}/building_config.h"
)
