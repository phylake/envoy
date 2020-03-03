#include "envoy/config/filter/http/header_size/v2/header_size.pb.validate.h"

#include "envoy/registry/registry.h"

#include "extensions/filters/http/common/factory_base.h"
#include "extensions/filters/http/header_size/header_size.h"
#include "extensions/filters/http/well_known_names.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace HeaderSize {

class HeaderSizeFactory
    : public Common::FactoryBase<envoy::config::filter::http::header_size::v2::HeaderSize,
                                 envoy::config::filter::http::header_size::v2::HeaderSizePerRoute> {
public:
  HeaderSizeFactory() : FactoryBase("envoy.filters.http.header_size") {}

private:
  Http::FilterFactoryCb createFilterFactoryFromProtoTyped(
      const envoy::config::filter::http::header_size::v2::HeaderSize& proto_config,
      const std::string& stats_prefix, Server::Configuration::FactoryContext& context) {
    ASSERT(proto_config.has_max_bytes());

    auto filter_config = std::make_shared<HeaderSizeConfig>(
        proto_config, stats_prefix, context.scope());

    return [filter_config](Http::FilterChainFactoryCallbacks& callbacks) -> void {
      callbacks.addStreamDecoderFilter(std::make_shared<HeaderSize>(filter_config));
    };
  }

  Router::RouteSpecificFilterConfigConstSharedPtr
  createRouteSpecificFilterConfigTyped(
      const envoy::config::filter::http::header_size::v2::HeaderSizePerRoute& proto_config,
      Server::Configuration::ServerFactoryContext&, ProtobufMessage::ValidationVisitor&) {
    return std::make_shared<const HeaderSizeSettings>(proto_config);
  }
};

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
