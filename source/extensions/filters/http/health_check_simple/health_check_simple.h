#pragma once

#include "envoy/http/filter.h"
#include "envoy/server/filter_config.h"
#include "envoy/api/v2/route/route.pb.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace HealthCheckSimple {

/**
 * A simpler health check filter
 */
class HealthCheckSimpleFilter : public Http::StreamDecoderFilter {
public:
  HealthCheckSimpleFilter(envoy::api::v2::route::HeaderMatcher header_data,
                            Server::Configuration::FactoryContext& context);
  ~HealthCheckSimpleFilter();

  // Http::StreamFilterBase
  void onDestroy() override;

  // Http::StreamDecoderFilter
  Http::FilterHeadersStatus decodeHeaders(Http::HeaderMap& headers, bool end_stream) override;
  Http::FilterDataStatus decodeData(Buffer::Instance& data, bool end_stream) override;
  Http::FilterTrailersStatus decodeTrailers(Http::HeaderMap& trailers) override;
  void setDecoderFilterCallbacks(Http::StreamDecoderFilterCallbacks& callbacks) override;

private:
  envoy::api::v2::route::HeaderMatcher header_proto_;
  Server::Configuration::FactoryContext& context_;
  Http::StreamDecoderFilterCallbacks* callbacks_{};
};

} // namespace HealthCheckSimple
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy
