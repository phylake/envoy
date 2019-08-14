#include "extensions/filters/listener/ip_allow_deny/ip_allow_deny.h"

#include "envoy/network/listen_socket.h"

#include "common/network/cidr_range.h"

namespace Envoy {
namespace Extensions {
namespace ListenerFilters {
namespace IpAllowDeny {

IpFilter::IpFilter(const envoy::config::filter::network::ip_allow_deny::v2::IpAllowDeny& config) {
  if (config.allow_cidrs_size() > 0) {
    allow_list_ = std::make_shared<Network::Address::IpList>(config.allow_cidrs());
  }

  if (config.deny_cidrs_size() > 0) {
    deny_list_ = std::make_shared<Network::Address::IpList>(config.deny_cidrs());
  }
}

Network::FilterStatus IpFilter::onAccept(Network::ListenerFilterCallbacks& cb) {
  ENVOY_LOG(debug, "ip: New connection accepted");

  Network::ConnectionSocket& socket = cb.socket();

  if (allow_list_ && !allow_list_->contains(*socket.remoteAddress())) {
    cb.socket().close();
    return Network::FilterStatus::StopIteration;
  }

  if (deny_list_ && deny_list_->contains(*socket.remoteAddress())) {
    cb.socket().close();
    return Network::FilterStatus::StopIteration;
  }

  return Network::FilterStatus::Continue;
}

} // namespace IpAllowDeny
} // namespace ListenerFilters
} // namespace Extensions
} // namespace Envoy
