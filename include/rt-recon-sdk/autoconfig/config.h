#ifndef RT_RECON_SDK_CONFIG_H
#define RT_RECON_SDK_CONFIG_H

#include <string>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <yaml-cpp/yaml.h>

#include "logger.h"

namespace rtrs {

struct DatabaseConfig {
  std::string host;
  uint32_t port;
  std::string org;
  std::string token;
  std::string bucket;
  std::string data_id;
};

/**
 * RF configuration parameters
 */
struct RfConfig {
  std::string device_name;
  std::string device_args;
  double      tx_freq_hz;
  double      rx_freq_hz;
  double      srate_hz;
  double      tx_gain_db;
  double      rx_gain_db;
};

/**
 * SSB configuration parameters
 */
struct SsbConfig {
  std::string pattern;
  uint32_t    scs_khz;
  uint32_t    periodicity_ms;
  double      f_offset_hz;
  double      ssb_freq_offset_hz;
  float       beta_pss;
  float       beta_sss;
  float       beta_pbch;
  float       beta_pbch_dmrs;
};

/**
 * Attack configuration parameters
 */
struct AttackConfig {
  uint32_t  target_pci;
  bool      scan_for_target;

  bool      modify_coreset0_idx;
  bool      modify_ss0_idx;
  bool      modify_cell_barred;
  bool      modify_intra_freq_resel;

  uint32_t  coreset0_idx_value;
  uint32_t  ss0_idx_value;
  bool      cell_barred_value;
  bool      intra_freq_resel_value;

  double    tx_power_db;
  double    tx_power_offset_db;
  bool      continuous_tx;

  uint64_t  max_bursts;
  uint32_t  burst_interval_us;
  uint32_t  burst_length_ms;
};

/**
 * Operational parameters
 */
struct OperationalConfig {
  double scan_duration_sec;
  std::string log_level;
  std::string log_file;
  bool save_samples;
  std::string samples_file;
};

/**
 * Complete configuration structure
 */
struct Config {
  DatabaseConfig database;
  RfConfig rf;
  SsbConfig ssb;
  AttackConfig attack;
  OperationalConfig operation;
};

/**
 * Configuration parser with autoconfiguration support
 */
class ConfigParser {
public:
  static bool load_from_file(const std::string& filename, Config& config);
  static bool load_from_influxdb(Config& config);
  static bool validate(const Config& config);
  static void print(const Config& config);
};

} // namespace rtrs

#endif // RT_RECON_SDK_CONFIG_H
