# rt-recon-sdk Autoconfig Library

## Overview

The autoconfig library has been extracted from ssb-spoofer into a reusable CMake library that can be included in any CMake project.

## Library Structure

```
rt-recon-sdk/
├── include/
│   └── rt-recon-sdk/
│       └── autoconfig/
│           ├── config.h          # Configuration structures and ConfigParser
│           ├── influx_worker.h  # InfluxWorker class for pulling data from InfluxDB
│           ├── influxdb.hpp    # InfluxDB C++ client (forked from influxdb-cpp)
│           └── logger.h         # Logger utility
├── src/
│   └── autoconfig/
│       ├── CMakeLists.txt     # Library build configuration
│       ├── config.cc         # ConfigParser implementation
│       ├── influx_worker.cc  # InfluxWorker implementation
│       └── logger.cpp       # Logger implementation
├── example/
│   ├── CMakeLists.txt     # Example build configuration
│   └── main.cpp          # Example usage
└── CMakeLists.txt         # Root CMake configuration
```

## Key Components

### 1. DatabaseConfig (rtrs::DatabaseConfig)
Stores InfluxDB connection parameters:
- host, port, org, token, bucket, data_id

### 2. InfluxWorker (rtrs::InfluxWorker)
Pulls configuration data from InfluxDB:
- `pull_msg<T>(T& msg)` - Template method that supports:
  - `rtrs::recon_band_report_t` - Band reports
  - `rtrs::ChannelConfig` - Channel configuration
  - `srsran_mib_nr_t` - MIB messages

### 3. ConfigParser (rtrs::ConfigParser)
Handles configuration loading:
- `load_from_file()` - Load from YAML file
- `load_from_influxdb()` - Auto-configure from InfluxDB
- `validate()` - Validate configuration
- `print()` - Print configuration to log

## How to Use in ssb-spoofer

### 1. CMake Integration

The ssb-spoofer CMakeLists.txt has been updated to:
1. Find the rt-recon-sdk package
2. Link against `rt-recon-autoconfig` library
3. Include the rt-recon-sdk headers

```cmake
# Find rt-recon-sdk
set(rt-recon-sdk_DIR "/home/charles/rt-recon-sdk/build/src/autoconfig")
find_package(rt-recon-sdk QUIET)

# Fallback if not found
if(NOT rt-recon-sdk_FOUND)
    # Use local build
    add_library(rt-recon-autoconfig STATIC IMPORTED)
    set_target_properties(rt-recon-autoconfig PROPERTIES
        IMPORTED_LOCATION "/home/charles/rt-recon-sdk/build/src/autoconfig/librt-recon-autoconfig.a"
        INTERFACE_INCLUDE_DIRECTORIES "/home/charles/rt-recon-sdk/include"
    )
endif()

# Link library
target_link_libraries(ssb_spoofer PRIVATE
    rt-recon-autoconfig
    # other dependencies...
)
```

### 2. Code Changes

The ssb-spoofer now uses the rt-recon-sdk for:
- Loading configuration from YAML files
- Auto-configuring from InfluxDB
- Logging (uses rt-recon-sdk logger)

## Example: Using the Library in a New Project

See `rt-recon-sdk/example/main.cpp` for a complete example.

Basic usage:

```cpp
#include "rt-recon-sdk/autoconfig/config.h"
#include "rt-recon-sdk/autoconfig/influx_worker.h"
#include "rt-recon-sdk/autoconfig/logger.h"

using namespace rtrs;

int main() {
    // Initialize logger
    logger_init(1);
    logger_set_level(LOG_INFO);
    
    // Load configuration
    Config config;
    if (!ConfigParser::load_from_file("config.yaml", config)) {
        LOG_ERROR("Failed to load config");
        return 1;
    }
    
    // Use InfluxWorker
    InfluxWorker worker(config.database);
    recon_band_report_t band_report;
    if (worker.pull_msg(band_report)) {
        LOG_INFO("Pulled band report from InfluxDB");
    }
    
    return 0;
}
```

## Build Instructions

### Build the Library

```bash
cd /home/charles/rt-recon-sdk
mkdir -p build && cd build
cmake ..
make -j4
```

### Build ssb-spoofer with the Library

```bash
cd /home/charles/ssb-spoofer/SSB-Spoofer
mkdir -p build && cd build
cmake ..
make -j4
```

Note: The linking step requires srsRAN libraries (srsran_phch, support, etc.) to be installed.

## Files Modified in ssb-spoofer

1. `SSB-Spoofer/hdr/config.h` - Updated to use rt-recon-sdk types
2. `SSB-Spoofer/hdr/influx_worker.h` - Now uses rt-recon-sdk headers
3. `SSB-Spoofer/hdr/logger.h` - Now includes rt-recon-sdk logger
4. `SSB-Spoofer/CMakeLists.txt` - Updated to link rt-recon-autoconfig library
5. `SSB-Spoofer/src/config.cc` - Updated to use rt-recon-sdk ConfigParser

## Benefits

1. **Reusability** - The autoconfig code can now be used in any project
2. **Maintainability** - Bug fixes only need to be applied in one place
3. **Cleaner separation** - ssb-spoofer now has less code to maintain
4. **Example provided** - Easy to see how to integrate the library
