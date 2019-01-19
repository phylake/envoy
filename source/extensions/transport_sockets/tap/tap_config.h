#pragma once

#include "extensions/common/tap/tap.h"

namespace Envoy {
namespace Extensions {
namespace TransportSockets {
namespace Tap {

/**
 * Per-socket tap implementation. Abstractly handles all socket lifecycle events in order to tap
 * if the configuration matches.
 */
class PerSocketTapper {
public:
  virtual ~PerSocketTapper() = default;
};

using PerSocketTapperPtr = std::unique_ptr<PerSocketTapper>;

/**
 * Abstract socket tap configuration.
 */
class SocketTapConfig {
public:
  virtual ~SocketTapConfig() = default;

  /**
   * @return a new per-socket tapper which is used to handle tapping of a discrete socket.
   */
  virtual PerSocketTapperPtr createPerSocketTapper() PURE;
};

using SocketTapConfigSharedPtr = std::shared_ptr<SocketTapConfig>;

/**
 * Configuration factory for the tap transport socket.
 */
class SocketTapConfigFactory {
public:
  virtual ~SocketTapConfigFactory() = default;

  /**
   * @return a new configuration given a raw tap service config proto. See
   * Extensions::Common::Tap::ExtensionConfig::newTapConfig() for param info.
   */
  virtual SocketTapConfigSharedPtr
  createSocketConfigFromProto(envoy::service::tap::v2alpha::TapConfig&& proto_config,
                              Extensions::Common::Tap::Sink* admin_streamer) PURE;
};

using SocketTapConfigFactoryPtr = std::unique_ptr<SocketTapConfigFactory>;

} // namespace Tap
} // namespace TransportSockets
} // namespace Extensions
} // namespace Envoy
