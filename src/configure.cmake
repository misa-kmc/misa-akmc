## generate configure file
if (EAM_POT_EBNABLE_FLAG)
    set(EAM_POT_ENABLED ON)
endif ()

if (KMC_DEBUG_ENABLE_FLAG)
    set(KMC_DEBUG_MODE ON)
endif ()

if (KMC_RAND MATCHES "LCG")
    set(RAND_LCG TRUE)
elseif (KMC_RAND MATCHES "MT")
    set(RAND_MT TRUE)
elseif (KMC_RAND MATCHES "STC")
    set(RAND_STC TRUE)
elseif (KMC_RAND MATCHES "xoshiro")
    set(RAND_XOSHIRO TRUE)
elseif (KMC_RAND MATCHES "REAL")
    #    set(RAND_LINUX_REAL TRUE)
    MESSAGE(SEND_ERROR "real rand number is not currently supported")
else ()
    MESSAGE(SEND_ERROR "unsupported random number generation method ${KMC_RAND}")
endif ()

configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/building_config.in"
        "${CMAKE_CURRENT_SOURCE_DIR}/building_config.h"
)
