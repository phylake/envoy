#pragma once

#include "envoy/config/filter/network/ip_allow_deny/v2/ip_allow_deny.pb.h"

#include "common/network/cidr_range.h"

#include "envoy/http/filter.h"
#include "envoy/stats/scope.h"
#include "envoy/stats/stats_macros.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace IpAllowDeny {

/**
 * All stats for the filter. @see stats_macros.h
 */
// clang-format off
#define IP_ALLOW_DENY_FILTER_STATS(COUNTER)                                                           \
  COUNTER(denied)
// clang-format on

struct IpAllowDenyStats {
  IP_ALLOW_DENY_FILTER_STATS(GENERATE_COUNTER_STRUCT)
};

class IpAllowDenySettings : public Router::RouteSpecificFilterConfig {
public:
  IpAllowDenySettings(const envoy::config::filter::network::ip_allow_deny::v2::IpAllowDeny&);

  std::shared_ptr<Network::Address::IpList> allow_list() const;
  std::shared_ptr<Network::Address::IpList> deny_list() const;

private:
  std::shared_ptr<Network::Address::IpList> allow_list_;
  std::shared_ptr<Network::Address::IpList> deny_list_;
};

class IpAllowDenyConfig {
public:
  IpAllowDenyConfig(const envoy::config::filter::network::ip_allow_deny::v2::IpAllowDeny& proto_config,
                    const std::string& stats_prefix, Stats::Scope& scope);

  IpAllowDenyStats& stats() { return stats_; }
  const IpAllowDenySettings* settings() const { return &settings_; }

private:
  IpAllowDenyStats stats_;
  const IpAllowDenySettings settings_;
};

typedef std::shared_ptr<IpAllowDenyConfig> IpAllowDenyConfigSharedPtr;

class IpAllowDeny : public Http::StreamDecoderFilter {
public:
  IpAllowDeny(IpAllowDenyConfigSharedPtr config);
  ~IpAllowDeny();

  static IpAllowDenyStats generateStats(const std::string& prefix, Stats::Scope& scope);

  // Http::StreamFilterBase
  void onDestroy() override;

  // Http::StreamDecoderFilter
  Http::FilterHeadersStatus decodeHeaders(Http::HeaderMap& headers, bool end_stream) override;
  Http::FilterDataStatus decodeData(Buffer::Instance& data, bool end_stream) override;
  Http::FilterTrailersStatus decodeTrailers(Http::HeaderMap& trailers) override;
  void setDecoderFilterCallbacks(Http::StreamDecoderFilterCallbacks& callbacks) override;

private:
  void initConfig();

  IpAllowDenyConfigSharedPtr config_;
  const IpAllowDenySettings* settings_;
  Http::StreamDecoderFilterCallbacks* callbacks_{};
  bool config_initialized_{};
};

} // namespace IpAllowDeny
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy

