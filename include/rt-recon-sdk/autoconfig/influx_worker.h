#ifndef RT_RECON_SDK_INFLUX_WORKER_H
#define RT_RECON_SDK_INFLUX_WORKER_H

#include "config.h"
#include "influxdb.hpp"
#include "logger.h"
#include <mutex>
#include <vector>
#include <queue>
#include <condition_variable>
#include <type_traits>
#include <variant>
#include <unordered_map>
#include <string>

// srsran types - these need srsran headers
#include "srsran/phy/gnb/gnb_dl.h"
#include "srsran/srslog/srslog.h"

namespace rtrs {

// Definitions for custom pullable structs
typedef struct influx_band_report_s {
    uint16_t band;
    uint32_t nof_prb;
    uint32_t offset_to_carrier;
    srsran_subcarrier_spacing_t scs_common;
    srsran_subcarrier_spacing_t scs_ssb;
    uint32_t dl_arfcn;
    uint32_t ul_arfcn;
    uint32_t ssb_arfcn;
    double dl_freq;
    double ul_freq;
    double ssb_freq;
    srsran_ssb_pattern_t ssb_pattern;
    double sample_rate;
    double uplink_cfo;
    double downlink_cfo;
} recon_band_report_t;

struct ChannelConfig {
  double rx_frequency;
  double tx_frequency;
  double rx_offset;
  double tx_offset;
  double rx_gain;
  double tx_gain;
  bool   enabled;
};

class InfluxWorker
{
public:
  explicit InfluxWorker(const DatabaseConfig config_);
  ~InfluxWorker() = default;

  // Function to pull messages from influxDB
  template <typename T>
  bool pull_msg(T& msg)
  {
    if constexpr (std::is_same_v<T, recon_band_report_t>) {
      return recv_band_report(msg);
    }
    else if constexpr (std::is_same_v<T, ChannelConfig>) {
      return recv_channel_config(msg);
    }
    else if constexpr (std::is_same_v<T, srsran_mib_nr_t>) {
      return recv_mib(msg);
    }
    else {
      static_assert(std::is_same_v<T, void>, "Unsupported type passed to pull_msg()");
    }
  }

private:
  influxdb_cpp::server_info influx_server_info;
  std::string data_id;

  bool recv_band_report(recon_band_report_t& report);
  bool recv_channel_config(ChannelConfig& ch);
  bool recv_mib(srsran_mib_nr_t& mib);

  static std::unordered_map<std::string, std::string> parse_flux_fields(const std::string& resp);
  static void log_fields_map(const std::unordered_map<std::string, std::string>& fields);
};

} // namespace rtrs

#endif // RT_RECON_SDK_INFLUX_WORKER_H
