#pragma once

#include "envoy/network/filter.h"

#include "common/network/cidr_range.h"
#include "common/common/logger.h"

#include "envoy/config/filter/network/ip_allow_deny/v2/ip_allow_deny.pb.h"

namespace Envoy {
namespace Extensions {
namespace ListenerFilters {
namespace IpAllowDeny {

/**
 * Implementation of an IP allow/deny listener filter.
 */
class IpFilter : public Network::ListenerFilter, Logger::Loggable<Logger::Id::filter> {
public:
  IpFilter(const envoy::config::filter::network::ip_allow_deny::v2::IpAllowDeny&);

  // Network::ListenerFilter
  Network::FilterStatus onAccept(Network::ListenerFilterCallbacks& cb) override;

private:
  std::shared_ptr<Network::Address::IpList> allow_list_;
  std::shared_ptr<Network::Address::IpList> deny_list_;
};

} // namespace IpAllowDeny
} // namespace ListenerFilters
} // namespace Extensions
} // namespace Envoy
