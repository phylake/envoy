#include "extensions/filters/listener/ip_block/ip_block.h"

#include "envoy/network/listen_socket.h"

#include "common/network/cidr_range.h"

namespace Envoy {
namespace Extensions {
namespace ListenerFilters {
namespace IpBlock {

IpBlockFilter::IpBlockFilter(const envoy::config::filter::network::ip_block::v2::IpBlock& config) : ip_list_(config.cidrs()) {}

Network::FilterStatus IpBlockFilter::onAccept(Network::ListenerFilterCallbacks& cb) {
  ENVOY_LOG(debug, "ip_block: New connection accepted");

  Network::ConnectionSocket& socket = cb.socket();
  if (ip_list_.contains(*socket.remoteAddress())) {
    cb.socket().close();
    return Network::FilterStatus::StopIteration;
  }

  return Network::FilterStatus::Continue;
}

} // namespace IpBlock
} // namespace ListenerFilters
} // namespace Extensions
} // namespace Envoy
