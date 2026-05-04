CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
project(rt-recon-sdk CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

set(RT_RECON_SDK_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Create config file for find_package
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/rt-recon-sdkConfigVersion.cmake"
    VERSION 1.0.0
    COMPATIBILITY SameMajorVersion
)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/rt-recon-sdkConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/rt-recon-sdkConfig.cmake"
    @ONLY
)

# Add the autoconfig library
add_subdirectory(src/autoconfig)

# Install targets
install(EXPORT rt-recon-sdkTargets
    FILE rt-recon-sdkTargets.cmake
    NAMESPACE rt-recon-sdk::
    DESTINATION lib/cmake/rt-recon-sdk
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/rt-recon-sdkConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/rt-recon-sdkConfigVersion.cmake"
    DESTINATION lib/cmake/rt-recon-sdk
)
