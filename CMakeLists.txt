CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
project(rt-recon-sdk CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

set(RT_RECON_SDK_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add the autoconfig library
add_subdirectory(src/autoconfig)
