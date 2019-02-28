#pragma once

#include "envoy/config/filter/http/header_size/v2/header_size.pb.h"

#include "extensions/filters/http/common/factory_base.h"
#include "extensions/filters/http/well_known_names.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace HeaderSize {

/**
 * Config registration for the header size filter.
 */
class HeaderSizeFactory
    : public Common::FactoryBase<envoy::config::filter::http::header_size::v2::HeaderSize,
                                 envoy::config::filter::http::header_size::v2::HeaderSizePerRoute> {
public:
  HeaderSizeFactory() : FactoryBase(HttpFilterNames::get().HeaderSize) {}

private:
  Http::FilterFactoryCb createFilterFactoryFromProtoTyped(
      const envoy::config::filter::http::header_size::v2::HeaderSize& proto_config,
      const std::string& stats_prefix, Server::Configuration::FactoryContext& context) override;

  Router::RouteSpecificFilterConfigConstSharedPtr createRouteSpecificFilterConfigTyped(
      const envoy::config::filter::http::header_size::v2::HeaderSizePerRoute&,
      Server::Configuration::FactoryContext&) override;
};

} // namespace HeaderSize
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy
