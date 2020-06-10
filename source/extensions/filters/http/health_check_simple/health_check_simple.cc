#include "common/http/header_utility.h"
#include "envoy/http/codes.h"
#include "extensions/filters/http/health_check_simple/health_check_simple.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace HealthCheckSimple {

HealthCheckSimpleFilter::HealthCheckSimpleFilter(envoy::config::route::v3::HeaderMatcher header_proto,
                                                   Server::Configuration::FactoryContext& context)
    : header_proto_(header_proto), context_(context) {}

HealthCheckSimpleFilter::~HealthCheckSimpleFilter() {}

Http::FilterHeadersStatus HealthCheckSimpleFilter::decodeHeaders(Http::HeaderMap& headers, bool) {
  if (Http::HeaderUtility::matchHeaders(headers, header_proto_)) {
    if (context_.healthCheckFailed()) {
      callbacks_->sendLocalReply(Http::Code::InternalServerError, "", nullptr, absl::nullopt, "");
    } else {
      callbacks_->sendLocalReply(Http::Code::OK, "", nullptr, absl::nullopt, "");
    }
    return Http::FilterHeadersStatus::StopIteration;
  }
  return Http::FilterHeadersStatus::Continue;
}

Http::FilterDataStatus HealthCheckSimpleFilter::decodeData(Buffer::Instance&, bool) {
  return Http::FilterDataStatus::Continue;
}

Http::FilterTrailersStatus HealthCheckSimpleFilter::decodeTrailers(Http::HeaderMap&) {
  return Http::FilterTrailersStatus::Continue;
}

void HealthCheckSimpleFilter::onDestroy() {}

void HealthCheckSimpleFilter::setDecoderFilterCallbacks(Http::StreamDecoderFilterCallbacks& callbacks) {
  callbacks_ = &callbacks;
}

} // namespace HealthCheckSimple
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy
