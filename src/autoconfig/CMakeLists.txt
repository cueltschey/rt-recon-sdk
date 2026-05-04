set(AUTOCONFIG_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(AUTOCONFIG_SOURCES
    ${AUTOCONFIG_SRC_DIR}/influx_worker.cc
    ${AUTOCONFIG_SRC_DIR}/logger.cpp
    ${AUTOCONFIG_SRC_DIR}/config.cc
)

# Create the autoconfig library
add_library(rt-recon-autoconfig STATIC ${AUTOCONFIG_SOURCES})

target_include_directories(rt-recon-autoconfig PUBLIC
    $<BUILD_INTERFACE:${RT_RECON_SDK_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:include>
)

# Link required dependencies
target_link_libraries(rt-recon-autoconfig PUBLIC
    srsran_phch
    srsran_phy
    srsran_common
    support
    srslog
    srsran_rf
    pthread
    yaml-cpp
)

# Install targets
install(TARGETS rt-recon-autoconfig
    EXPORT rt-recon-sdkTargets
    DESTINATION lib
)
install(DIRECTORY ${RT_RECON_SDK_INCLUDE_DIR}/rt-recon-sdk DESTINATION include)
