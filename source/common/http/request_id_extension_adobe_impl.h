#pragma once

#include "envoy/http/request_id_extension.h"

#include "common/runtime/runtime_impl.h"

namespace Envoy {
namespace Http {

class AdobeRequestIDExtension : public RequestIDExtension {
public:
  explicit AdobeRequestIDExtension(Random::RandomGenerator& random) : random_(random) {}

  void set(RequestHeaderMap& request_headers, bool force) override;
  void setInResponse(ResponseHeaderMap& response_headers,
                     const RequestHeaderMap& request_headers) override;
  bool modBy(const RequestHeaderMap& request_headers, uint64_t& out, uint64_t mod) override;
  TraceStatus getTraceStatus(const RequestHeaderMap& request_headers) override;
  void setTraceStatus(RequestHeaderMap& request_headers, TraceStatus status) override;

private:
  // Reference to the random generator used to generate new request IDs
  Random::RandomGenerator& random_;
};

} // namespace Http
} // namespace Envoy
