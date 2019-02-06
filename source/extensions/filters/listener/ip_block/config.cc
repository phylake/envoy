#include <string>

#include "envoy/registry/registry.h"
#include "envoy/server/filter_config.h"

#include "extensions/filters/listener/ip_block/ip_block.h"
#include "extensions/filters/listener/well_known_names.h"

namespace Envoy {
namespace Extensions {
namespace ListenerFilters {
namespace IpBlock {

/**
 * Config registration for the ip block filter. @see NamedNetworkFilterConfigFactory.
 */
class IpBlockConfigFactory : public Server::Configuration::NamedListenerFilterConfigFactory {
public:
  // NamedListenerFilterConfigFactory
  Network::ListenerFilterFactoryCb
  createFilterFactoryFromProto(const Protobuf::Message& msg,
                               Server::Configuration::ListenerFactoryContext&) override {

    envoy::config::filter::network::ip_block::v2::IpBlock config;
    config.CopyFrom(msg);

    return [config](Network::ListenerFilterManager& filter_manager) -> void {
      filter_manager.addAcceptFilter(std::make_unique<IpBlockFilter>(config));
    };
  }

  ProtobufTypes::MessagePtr createEmptyConfigProto() override {
    return std::make_unique<envoy::config::filter::network::ip_block::v2::IpBlock>();
  }

  std::string name() override { return ListenerFilterNames::get().IpBlock; }
};

/**
 * Static registration for the ip block filter. @see RegisterFactory.
 */
static Registry::RegisterFactory<IpBlockConfigFactory,
                                 Server::Configuration::NamedListenerFilterConfigFactory>
    registered_;

} // namespace IpBlock
} // namespace ListenerFilters
} // namespace Extensions
} // namespace Envoy
