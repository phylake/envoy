#include "extensions/filters/http/ip_allow_deny/ip_allow_deny.h"

#include "envoy/http/codes.h"
#include "envoy/stats/scope.h"

#include "common/common/assert.h"
#include "common/http/headers.h"

#include "extensions/filters/http/well_known_names.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace IpAllowDeny {

IpAllowDenySettings::IpAllowDenySettings(
    const envoy::config::filter::network::ip_allow_deny::v2::IpAllowDeny& proto_config) {

  if (proto_config.allow_cidrs_size() > 0) {
    allow_list_ = std::make_shared<Network::Address::IpList>(proto_config.allow_cidrs());
  }

  if (proto_config.deny_cidrs_size() > 0) {
    deny_list_ = std::make_shared<Network::Address::IpList>(proto_config.deny_cidrs());
  }
}

std::shared_ptr<Network::Address::IpList> IpAllowDenySettings::allow_list() const {
  return allow_list_;
}

std::shared_ptr<Network::Address::IpList> IpAllowDenySettings::deny_list() const {
  return deny_list_;
}

IpAllowDenyConfig::IpAllowDenyConfig(
    const envoy::config::filter::network::ip_allow_deny::v2::IpAllowDeny& proto_config,
    const std::string& stats_prefix, Stats::Scope& scope)
    : stats_(IpAllowDeny::generateStats(stats_prefix, scope)), settings_(proto_config) {}

IpAllowDeny::IpAllowDeny(IpAllowDenyConfigSharedPtr config)
    : config_(config), settings_(config->settings()) {}

IpAllowDeny::~IpAllowDeny() {}

void IpAllowDeny::initConfig() {
  ASSERT(!config_initialized_);
  config_initialized_ = true;

  if (!callbacks_->route() || !callbacks_->route()->routeEntry()) {
    return;
  }

  const std::string& name = "envoy.filters.http.ip_allow_deny";
  const auto* entry = callbacks_->route()->routeEntry();

  const IpAllowDenySettings* route_local =
      entry->perFilterConfigTyped<IpAllowDenySettings>(name)
          ? entry->perFilterConfigTyped<IpAllowDenySettings>(name)
          : entry->virtualHost().perFilterConfigTyped<IpAllowDenySettings>(name);

  settings_ = route_local ? route_local : settings_;
}

Http::FilterHeadersStatus IpAllowDeny::decodeHeaders(Http::HeaderMap&, bool) {
  initConfig();

  Network::Address::InstanceConstSharedPtr addr = callbacks_->connection()->remoteAddress();

  if (settings_->allow_list() &&
        !settings_->allow_list()->contains(*addr.get())) {

    callbacks_->sendLocalReply(Http::Code::Forbidden, "", nullptr, absl::nullopt, "");
    config_->stats().denied_.inc();
    return Http::FilterHeadersStatus::StopIteration;
  }

  if (settings_->deny_list() &&
        settings_->deny_list()->contains(*addr.get())) {

    callbacks_->sendLocalReply(Http::Code::Forbidden, "", nullptr, absl::nullopt, "");
    config_->stats().denied_.inc();
    return Http::FilterHeadersStatus::StopIteration;
  }

  return Http::FilterHeadersStatus::Continue;
}

Http::FilterDataStatus IpAllowDeny::decodeData(Buffer::Instance&, bool) {
  return Http::FilterDataStatus::Continue;
}

Http::FilterTrailersStatus IpAllowDeny::decodeTrailers(Http::HeaderMap&) {
  return Http::FilterTrailersStatus::Continue;
}

IpAllowDenyStats IpAllowDeny::generateStats(const std::string& prefix, Stats::Scope& scope) {
  std::string final_prefix = prefix + "ip_allow_deny.";
  return {IP_ALLOW_DENY_FILTER_STATS(POOL_COUNTER_PREFIX(scope, final_prefix))};
}

void IpAllowDeny::onDestroy() {}

void IpAllowDeny::setDecoderFilterCallbacks(Http::StreamDecoderFilterCallbacks& callbacks) {
  callbacks_ = &callbacks;
}

} // namespace IpAllowDeny
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy
