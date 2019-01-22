#include "extensions/filters/http/header_size/config.h"

#include <chrono>
#include <cstdint>
#include <string>

#include "envoy/config/filter/http/header_size/v2/header_size.pb.validate.h"
#include "envoy/registry/registry.h"

#include "common/config/filter_json.h"

#include "extensions/filters/http/header_size/header_size.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace HeaderSize {

Http::FilterFactoryCb HeaderSizeFactory::createFilterFactoryFromProtoTyped(
    const envoy::config::filter::http::header_size::v2::HeaderSize& proto_config,
    const std::string& stats_prefix, Server::Configuration::FactoryContext& context) {
  ASSERT(proto_config.has_max_bytes());

  HeaderSizeConfigSharedPtr filter_config(
      new HeaderSizeConfig(proto_config, stats_prefix, context.scope()));
  return [filter_config](Http::FilterChainFactoryCallbacks& callbacks) -> void {
    callbacks.addStreamDecoderFilter(std::make_shared<HeaderSize>(filter_config));
  };
}

Http::FilterFactoryCb
HeaderSizeFactory::createFilterFactory(const Json::Object& json_config,
                                         const std::string& stats_prefix,
                                         Server::Configuration::FactoryContext& context) {
  envoy::config::filter::http::header_size::v2::HeaderSize proto_config;
  Config::FilterJson::translateHeaderSizeFilter(json_config, proto_config);
  return createFilterFactoryFromProtoTyped(proto_config, stats_prefix, context);
}

Router::RouteSpecificFilterConfigConstSharedPtr
HeaderSizeFactory::createRouteSpecificFilterConfigTyped(
    const envoy::config::filter::http::header_size::v2::HeaderSizePerRoute& proto_config,
    Server::Configuration::FactoryContext&) {
  return std::make_shared<const HeaderSizeSettings>(proto_config);
}

/**
 * Static registration for the header filter. @see RegisterFactory.
 */
static Registry::RegisterFactory<HeaderSizeFactory,
                                 Server::Configuration::NamedHttpFilterConfigFactory>
    register_;

} // namespace HeaderSize
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy
