#include "envoy/config/filter/http/health_check_simple/v2/health_check_simple.pb.validate.h"
#include "envoy/api/v2/route/route.pb.h"

#include "envoy/registry/registry.h"

#include "extensions/filters/http/common/factory_base.h"
#include "extensions/filters/http/health_check_simple/health_check_simple.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace HealthCheckSimple {

class HealthCheckSimpleFactory
    : public Common::FactoryBase<envoy::config::filter::http::health_check_simple::v2::HealthCheckSimple> {
public:
  HealthCheckSimpleFactory() : FactoryBase("envoy.filters.http.health_check_simple") {}

private:
  Http::FilterFactoryCb createFilterFactoryFromProtoTyped(
      const envoy::config::filter::http::health_check_simple::v2::HealthCheckSimple& proto_config,
      const std::string&, Server::Configuration::FactoryContext& context) {
    ASSERT(proto_config.path().size() > 0);

    envoy::api::v2::route::HeaderMatcher header_proto;
    header_proto.set_name(":path");
    header_proto.set_exact_match(proto_config.path());

    return [header_proto, &context](Http::FilterChainFactoryCallbacks& callbacks) -> void {
      callbacks.addStreamDecoderFilter(std::make_shared<HealthCheckSimpleFilter>(header_proto, context));
    };
  }
};

/**
 * Static registration for the header filter. @see RegisterFactory.
 */
static Registry::RegisterFactory<HealthCheckSimpleFactory,
                                 Server::Configuration::NamedHttpFilterConfigFactory>
    register_;

} // namespace HealthCheckSimple
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy
