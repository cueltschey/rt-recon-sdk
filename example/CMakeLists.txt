# Example of using rt-recon-sdk autoconfig library

CMAKE_MINIMUM_REQUIRED(VERSION 3.5)
project(autoconfig_example CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

# Find the rt-recon-sdk
find_package(rt-recon-sdk REQUIRED)

# Create example executable
add_executable(autoconfig_example main.cpp)

target_include_directories(autoconfig_example PRIVATE
    ${RT_RECON_SDK_INCLUDE_DIRS}
)

target_link_libraries(autoconfig_example PRIVATE
    rt-recon-autoconfig
)
