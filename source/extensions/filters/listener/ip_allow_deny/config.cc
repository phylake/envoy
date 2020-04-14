#include <string>

#include "envoy/registry/registry.h"
#include "envoy/server/filter_config.h"

#include "extensions/filters/listener/ip_allow_deny/ip_allow_deny.h"
#include "extensions/filters/listener/well_known_names.h"

namespace Envoy {
namespace Extensions {
namespace ListenerFilters {
namespace IpAllowDeny {

/**
 * Config registration for the ip block filter. @see NamedNetworkFilterConfigFactory.
 */
class IpConfigFactory : public Server::Configuration::NamedListenerFilterConfigFactory {
public:
  // NamedListenerFilterConfigFactory
  Network::ListenerFilterFactoryCb
  createFilterFactoryFromProto(const Protobuf::Message& msg,
                               Server::Configuration::ListenerFactoryContext&) override {

    envoy::config::filter::network::ip_allow_deny::v2::IpAllowDeny config;
    config.CopyFrom(msg);

    return [config](Network::ListenerFilterManager& filter_manager) -> void {
      filter_manager.addAcceptFilter(std::make_unique<IpFilter>(config));
    };
  }

  ProtobufTypes::MessagePtr createEmptyConfigProto() override {
    return std::make_unique<envoy::config::filter::network::ip_allow_deny::v2::IpAllowDeny>();
  }

  std::string name() const override { return "envoy.listener.ip_allow_deny"; }
};

/**
 * Static registration for the ip block filter. @see RegisterFactory.
 */
static Registry::RegisterFactory<IpConfigFactory,
                                 Server::Configuration::NamedListenerFilterConfigFactory>
    registered_;

} // namespace IpAllowDeny
} // namespace ListenerFilters
} // namespace Extensions
} // namespace Envoy
