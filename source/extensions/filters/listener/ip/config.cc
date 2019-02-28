#include <string>

#include "envoy/registry/registry.h"
#include "envoy/server/filter_config.h"

#include "extensions/filters/listener/ip/ip.h"
#include "extensions/filters/listener/well_known_names.h"

namespace Envoy {
namespace Extensions {
namespace ListenerFilters {
namespace Ip {

/**
 * Config registration for the ip block filter. @see NamedNetworkFilterConfigFactory.
 */
class IpConfigFactory : public Server::Configuration::NamedListenerFilterConfigFactory {
public:
  // NamedListenerFilterConfigFactory
  Network::ListenerFilterFactoryCb
  createFilterFactoryFromProto(const Protobuf::Message& msg,
                               Server::Configuration::ListenerFactoryContext&) override {

    envoy::config::filter::network::ip::v2::Ip config;
    config.CopyFrom(msg);

    return [config](Network::ListenerFilterManager& filter_manager) -> void {
      filter_manager.addAcceptFilter(std::make_unique<IpFilter>(config));
    };
  }

  ProtobufTypes::MessagePtr createEmptyConfigProto() override {
    return std::make_unique<envoy::config::filter::network::ip::v2::Ip>();
  }

  std::string name() override { return ListenerFilterNames::get().Ip; }
};

/**
 * Static registration for the ip block filter. @see RegisterFactory.
 */
static Registry::RegisterFactory<IpConfigFactory,
                                 Server::Configuration::NamedListenerFilterConfigFactory>
    registered_;

} // namespace Ip
} // namespace ListenerFilters
} // namespace Extensions
} // namespace Envoy
