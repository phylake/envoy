#pragma once

#include "envoy/http/filter.h"
#include "envoy/server/filter_config.h"

#include "common/http/header_utility.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace HealthCheckSimple {

/**
 * A simpler health check filter
 */
class HealthCheckSimpleFilter : public Http::StreamDecoderFilter {
public:
  HealthCheckSimpleFilter(std::vector<Http::HeaderUtility::HeaderData> header_data,
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
  std::vector<Http::HeaderUtility::HeaderData> header_data_;
  Server::Configuration::FactoryContext& context_;
  Http::StreamDecoderFilterCallbacks* callbacks_{};
};

} // namespace HealthCheckSimple
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy
