#include "common/http/request_id_extension_adobe_impl.h"

#include <cstdint>
#include <string>

#include "envoy/http/header_map.h"

#include "common/common/utility.h"
#include "common/runtime/runtime_impl.h"

#include "absl/strings/string_view.h"

namespace Envoy {
namespace Http {

void AdobeRequestIDExtension::set(RequestHeaderMap& request_headers, bool force) {
  if (!force && request_headers.RequestId()) {
    return;
  }

  std::string uuid = random_.uuid();
  ASSERT(!uuid.empty());
  request_headers.setRequestId(uuid);
}

void AdobeRequestIDExtension::setInResponse(ResponseHeaderMap& response_headers,
                                           const RequestHeaderMap& request_headers) {
  if (request_headers.RequestId()) {
    response_headers.setRequestId(request_headers.RequestId()->value().getStringView());
  }
}

bool AdobeRequestIDExtension::modBy(const RequestHeaderMap&, uint64_t&, uint64_t) {
  return true;
}

TraceStatus AdobeRequestIDExtension::getTraceStatus(const RequestHeaderMap& request_headers) {
  if (request_headers.EnvoyTraceDecision() == nullptr) {
    return TraceStatus::NoTrace;
  }

  const std::string decision(request_headers.EnvoyTraceDecision()->value().getStringView());
  if (decision == "forced_sample") {
    return TraceStatus::Forced;
  } else if (decision == "client_sample") {
    return TraceStatus::Client;
  } else if (decision == "random_sample") {
    return TraceStatus::Sampled;
  }

  return TraceStatus::NoTrace;
}

void AdobeRequestIDExtension::setTraceStatus(RequestHeaderMap& request_headers, TraceStatus status) {
  switch (status) {
  case TraceStatus::Forced:
    request_headers.setEnvoyTraceDecision("forced_sample");
    break;
  case TraceStatus::Client:
    request_headers.setEnvoyTraceDecision("client_sample");
    break;
  case TraceStatus::Sampled:
    request_headers.setEnvoyTraceDecision("random_sample");
    break;
  case TraceStatus::NoTrace:
    break;
  }
}

} // namespace Http
} // namespace Envoy
