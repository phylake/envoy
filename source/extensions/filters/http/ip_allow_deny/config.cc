#include "envoy/config/filter/network/ip_allow_deny/v2/ip_allow_deny.pb.validate.h"

#include "envoy/registry/registry.h"

#include "extensions/filters/http/common/factory_base.h"
#include "extensions/filters/http/ip_allow_deny/ip_allow_deny.h"
#include "extensions/filters/http/well_known_names.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace IpAllowDeny {

class IpAllowDenyFactory
    : public Common::FactoryBase<envoy::config::filter::network::ip_allow_deny::v2::IpAllowDeny> {
public:
  IpAllowDenyFactory() : FactoryBase("envoy.filters.http.ip_allow_deny") {}

private:
  Http::FilterFactoryCb createFilterFactoryFromProtoTyped(
      const envoy::config::filter::network::ip_allow_deny::v2::IpAllowDeny& proto_config,
      const std::string& stats_prefix, Server::Configuration::FactoryContext& context) {

    auto filter_config = std::make_shared<IpAllowDenyConfig>(
        proto_config, stats_prefix, context.scope());

    return [filter_config](Http::FilterChainFactoryCallbacks& callbacks) -> void {
      callbacks.addStreamDecoderFilter(std::make_shared<IpAllowDeny>(filter_config));
    };
  }

  Router::RouteSpecificFilterConfigConstSharedPtr
  createRouteSpecificFilterConfigTyped(
      const envoy::config::filter::network::ip_allow_deny::v2::IpAllowDeny& proto_config,
      Server::Configuration::FactoryContext&) {
    return std::make_shared<const IpAllowDenySettings>(proto_config);
  }
};

/**
 * Static registration for the filter. @see RegisterFactory.
 */
static Registry::RegisterFactory<IpAllowDenyFactory,
                                 Server::Configuration::NamedHttpFilterConfigFactory>
    register_;

} // namespace IpAllowDeny
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy
