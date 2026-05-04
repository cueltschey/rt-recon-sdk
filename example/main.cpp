/**
 * Example: Using rt-recon-sdk autoconfig library
 * 
 * This example demonstrates how to:
 * 1. Load configuration from YAML file
 * 2. Auto-configure from InfluxDB
 * 3. Use the InfluxWorker to pull different message types
 */

#include "rt-recon-sdk/autoconfig/config.h"
#include "rt-recon-sdk/autoconfig/influx_worker.h"
#include "rt-recon-sdk/autoconfig/logger.h"

#include <iostream>
#include <cstdlib>

using namespace rtrs;

int main(int argc, char** argv) {
    // Initialize logger
    logger_init(1);
    logger_set_level(LOG_INFO);
    
    LOG_INFO("rt-recon-sdk autoconfig example");
    
    // Check command line args
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <config.yaml>" << std::endl;
        std::cout << "\nThis example shows how to use the autoconfig library." << std::endl;
        std::cout << "The config file should have 'database' section with InfluxDB settings." << std::endl;
        std::cout << "Set 'enable_autoconfigure: true' to pull config from InfluxDB." << std::endl;
        return 1;
    }
    
    std::string config_file = argv[1];
    
    // Load configuration
    Config config;
    if (!ConfigParser::load_from_file(config_file, config)) {
        LOG_ERROR("Failed to load configuration from %s", config_file.c_str());
        return 1;
    }
    
    LOG_INFO("Configuration loaded successfully");
    ConfigParser::print(config);
    
    // Example: Use InfluxWorker to pull messages
    if (!config.database.host.empty() && !config.database.token.empty()) {
        LOG_INFO("Testing InfluxDB connection...");
        
        InfluxWorker worker(config.database);
        
        // Pull channel config
        ChannelConfig ch_config;
        if (worker.pull_msg(ch_config)) {
            LOG_INFO("Successfully pulled ChannelConfig from InfluxDB");
            LOG_INFO("  RX freq: %.3f MHz", ch_config.rx_frequency / 1e6);
            LOG_INFO("  TX freq: %.3f MHz", ch_config.tx_frequency / 1e6);
        } else {
            LOG_WARN("Failed to pull ChannelConfig (this is OK if no data exists)");
        }
        
        // Pull band report
        recon_band_report_t band_report;
        if (worker.pull_msg(band_report)) {
            LOG_INFO("Successfully pulled band report from InfluxDB");
            LOG_INFO("  Band: %u", band_report.band);
            LOG_INFO("  SSB freq: %.3f MHz", band_report.ssb_freq / 1e6);
        } else {
            LOG_WARN("Failed to pull band report (this is OK if no data exists)");
        }
    } else {
        LOG_INFO("No InfluxDB config found, skipping InfluxDB example");
    }
    
    LOG_INFO("Example completed successfully!");
    logger_shutdown();
    
    return 0;
}
