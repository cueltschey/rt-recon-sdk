# rt-recon-sdk Autoconfig Library

## Overview

The autoconfig library has been extracted from ssb-spoofer into a reusable CMake library that can be included in any CMake project using `FetchContent`.

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

## How to Use in Any CMake Project

### Option 1: Using FetchContent (Recommended)

Add this to your `CMakeLists.txt`:

```cmake
include(FetchContent)

# Use a specific commit hash for reproducibility
# Update this hash when you want to pull new changes
FetchContent_Declare(
    rt-recon-sdk
    GIT_REPOSITORY https://github.com/cueltschey/rt-recon-sdk.git
    GIT_TAG        main        # or use a commit hash like "a1b2c3d"
    GIT_SHALLOW   TRUE
)

message(STATUS "Fetching rt-recon-sdk from GitHub...")
FetchContent_MakeAvailable(rt-recon-sdk)

# Link against the library
target_link_libraries(your_target PRIVATE
    rt-recon-autoconfig
    # other dependencies...
)

# The include directories are automatically handled
```

**Benefits:**
- No need to manually clone or install the library
- Reproducible builds using specific commit hashes
- Automatic dependency management

### Option 2: Using find_package (Traditional)

```cmake
find_package(rt-recon-sdk REQUIRED)

target_link_libraries(your_target PRIVATE
    rt-recon-autoconfig
)
```

## Example: Using the Library

See `rt-recon-sdk/example/` for a complete example.

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

### Build the Library (Optional - FetchContent does this automatically)

```bash
cd /home/charles/rt-recon-sdk
mkdir -p build && cd build
cmake ..
make -j4
```

### Build ssb-spoofer with FetchContent

```bash
cd /home/charles/ssb-spoofer/SSB-Spoofer
mkdir -p build && cd build
cmake ..
make -j4
```

Note: The linking step requires srsRAN libraries (srsran_phch, support, etc.) to be installed.

## Files Modified in ssb-spoofer

1. `SSB-Spoofer/CMakeLists.txt` - Now uses FetchContent to download rt-recon-sdk from GitHub
2. `SSB-Spoofer/hdr/config.h` - Updated to use rt-recon-sdk
3. `SSB-Spoofer/hdr/influx_worker.h` - Now uses rt-recon-sdk headers
4. `SSB-Spoofer/hdr/logger.h` - Now includes rt-recon-sdk logger
5. `SSB-Spoofer/src/config.cc` - Updated to use rt-recon-sdk ConfigParser

## Benefits

1. **Reusability** - The autoconfig code can now be used in any project
2. **Maintainability** - Bug fixes only need to be applied in one place
3. **Cleaner separation** - ssb-spoofer now has less code to maintain
4. **FetchContent integration** - Easy to integrate with any CMake project
5. **Example provided** - Easy to see how to integrate the library
