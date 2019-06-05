#pragma once

#include "envoy/config/filter/http/header_size/v2/header_size.pb.h"
#include "envoy/http/filter.h"
#include "envoy/stats/scope.h"
#include "envoy/stats/stats_macros.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace HeaderSize {

/**
 * All stats for the header filter. @see stats_macros.h
 */
// clang-format off
#define ALL_HEADER_SIZE_FILTER_STATS(COUNTER)                                                           \
  COUNTER(hit_limit)
// clang-format on

/**
 * Wrapper struct for header filter stats. @see stats_macros.h
 */
struct HeaderSizeStats {
  ALL_HEADER_SIZE_FILTER_STATS(GENERATE_COUNTER_STRUCT)
};

class HeaderSizeSettings : public Router::RouteSpecificFilterConfig {
public:
  HeaderSizeSettings(const envoy::config::filter::http::header_size::v2::HeaderSize&);
  HeaderSizeSettings(const envoy::config::filter::http::header_size::v2::HeaderSizePerRoute&);

  bool disabled() const { return disabled_; }
  uint32_t maxBytes() const { return max_bytes_; }

private:
  bool disabled_;
  uint32_t max_bytes_;
};

/**
 * Configuration for the header size filter.
 */
class HeaderSizeConfig {
public:
  HeaderSizeConfig(const envoy::config::filter::http::header_size::v2::HeaderSize& proto_config,
                     const std::string& stats_prefix, Stats::Scope& scope);

  HeaderSizeStats& stats() { return stats_; }
  const HeaderSizeSettings* settings() const { return &settings_; }

private:
  HeaderSizeStats stats_;
  const HeaderSizeSettings settings_;
};

typedef std::shared_ptr<HeaderSizeConfig> HeaderSizeConfigSharedPtr;

/**
 * A filter that is capable of blocking requests if the sum of the size of its headers exceed a certain max_bytes
 */
class HeaderSize : public Http::StreamDecoderFilter {
public:
  HeaderSize(HeaderSizeConfigSharedPtr config);
  ~HeaderSize();

  static HeaderSizeStats generateStats(const std::string& prefix, Stats::Scope& scope);

  // Http::StreamFilterBase
  void onDestroy() override;

  // Http::StreamDecoderFilter
  Http::FilterHeadersStatus decodeHeaders(Http::HeaderMap& headers, bool end_stream) override;
  Http::FilterDataStatus decodeData(Buffer::Instance& data, bool end_stream) override;
  Http::FilterTrailersStatus decodeTrailers(Http::HeaderMap& trailers) override;
  void setDecoderFilterCallbacks(Http::StreamDecoderFilterCallbacks& callbacks) override;

private:
  void initConfig();

  HeaderSizeConfigSharedPtr config_;
  const HeaderSizeSettings* settings_;
  Http::StreamDecoderFilterCallbacks* callbacks_{};
  bool config_initialized_{};
};

} // namespace HeaderSize
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy
