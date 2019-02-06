#pragma once

#include "envoy/network/filter.h"

#include "common/network/cidr_range.h"
#include "common/common/logger.h"

#include "envoy/config/filter/network/ip_block/v2/ip_block.pb.h"

namespace Envoy {
namespace Extensions {
namespace ListenerFilters {
namespace IpBlock {

/**
 * Implementation of an IP blocking listener filter.
 */
class IpBlockFilter : public Network::ListenerFilter, Logger::Loggable<Logger::Id::filter> {
public:
  IpBlockFilter(const envoy::config::filter::network::ip_block::v2::IpBlock&);

  // Network::ListenerFilter
  Network::FilterStatus onAccept(Network::ListenerFilterCallbacks& cb) override;

private:
  Network::Address::IpList ip_list_;
};

} // namespace IpBlock
} // namespace ListenerFilters
} // namespace Extensions
} // namespace Envoy
