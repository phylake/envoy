#include "extensions/filters/http/header_size/header_size.h"

#include "envoy/http/codes.h"
#include "envoy/stats/scope.h"

#include "common/common/assert.h"
#include "common/http/headers.h"

#include "extensions/filters/http/well_known_names.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace HeaderSize {

HeaderSizeSettings::HeaderSizeSettings(
    const envoy::config::filter::http::header_size::v2::HeaderSize& proto_config)
    : disabled_(false),
      max_bytes_(proto_config.max_bytes().value()) {}

HeaderSizeSettings::HeaderSizeSettings(
    const envoy::config::filter::http::header_size::v2::HeaderSizePerRoute& proto_config)
    : disabled_(proto_config.disabled()),
      max_bytes_(
          proto_config.has_header_size()
              ? proto_config.header_size().max_bytes().value()
              : 0) {}

HeaderSizeConfig::HeaderSizeConfig(
    const envoy::config::filter::http::header_size::v2::HeaderSize& proto_config,
    const std::string& stats_prefix, Stats::Scope& scope)
    : stats_(HeaderSize::generateStats(stats_prefix, scope)), settings_(proto_config) {}

HeaderSize::HeaderSize(HeaderSizeConfigSharedPtr config)
    : config_(config), settings_(config->settings()) {}

HeaderSize::~HeaderSize() {}

void HeaderSize::initConfig() {
  ASSERT(!config_initialized_);
  config_initialized_ = true;

  if (!callbacks_->route() || !callbacks_->route()->routeEntry()) {
    return;
  }

  const std::string& name = "envoy.filters.http.header_size";
  const auto* entry = callbacks_->route()->routeEntry();

  const HeaderSizeSettings* route_local =
      entry->perFilterConfigTyped<HeaderSizeSettings>(name)
          ? entry->perFilterConfigTyped<HeaderSizeSettings>(name)
          : entry->virtualHost().perFilterConfigTyped<HeaderSizeSettings>(name);

  settings_ = route_local ? route_local : settings_;
}

Http::FilterHeadersStatus HeaderSize::decodeHeaders(Http::RequestHeaderMap& headers, bool) {
  initConfig();
  if (settings_->disabled()) {
    // The filter has been disabled for this route.
    return Http::FilterHeadersStatus::Continue;
  }

  if (headers.byteSize() > settings_->maxBytes()) {
    callbacks_->sendLocalReply(Http::Code::RequestHeaderFieldsTooLarge, "", nullptr, absl::nullopt, "");
    config_->stats().hit_limit_.inc();
    return Http::FilterHeadersStatus::StopIteration;
  }

  return Http::FilterHeadersStatus::Continue;
}

Http::FilterDataStatus HeaderSize::decodeData(Buffer::Instance&, bool) {
  return Http::FilterDataStatus::Continue;
}

Http::FilterTrailersStatus HeaderSize::decodeTrailers(Http::RequestTrailerMap&) {
  return Http::FilterTrailersStatus::Continue;
}

HeaderSizeStats HeaderSize::generateStats(const std::string& prefix, Stats::Scope& scope) {
  std::string final_prefix = prefix + "header_size.";
  return {ALL_HEADER_SIZE_FILTER_STATS(POOL_COUNTER_PREFIX(scope, final_prefix))};
}

void HeaderSize::onDestroy() {}

void HeaderSize::setDecoderFilterCallbacks(Http::StreamDecoderFilterCallbacks& callbacks) {
  callbacks_ = &callbacks;
}

} // namespace HeaderSize
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy
