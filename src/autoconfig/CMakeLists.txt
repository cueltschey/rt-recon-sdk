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

# Export targets for find_package
install(EXPORT rt-recon-sdkTargets
    FILE rt-recon-sdkTargets.cmake
    NAMESPACE rt-recon-sdk::
    DESTINATION lib/cmake/rt-recon-sdk
)

# Create config file for find_package
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/rt-recon-sdkConfigVersion.cmake"
    VERSION 1.0.0
    COMPATIBILITY SameMajorVersion
)

# Find the cmake directory relative to this file
# When fetched by FetchContent, the structure is different
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../cmake/rt-recon-sdkConfig.cmake.in")
    set(RT_RECON_SDK_CMAKE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")
elseif(EXISTS "${CMAKE_SOURCE_DIR}/cmake/rt-recon-sdkConfig.cmake.in")
    set(RT_RECON_SDK_CMAKE_DIR "${CMAKE_SOURCE_DIR}/cmake")
else()
    message(FATAL_ERROR "Cannot find rt-recon-sdkConfig.cmake.in")
endif()

configure_file(
    "${RT_RECON_SDK_CMAKE_DIR}/rt-recon-sdkConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/rt-recon-sdkConfig.cmake"
    @ONLY
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/rt-recon-sdkConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/rt-recon-sdkConfigVersion.cmake"
    DESTINATION lib/cmake/rt-recon-sdk
)
