#pragma once

// fixfix#include <fstream>

// fixfix#include "envoy/config/transport_socket/tap/v2alpha/tap.pb.h"
#include "envoy/data/tap/v2alpha/transport.pb.h"
#include "envoy/event/timer.h"
#include "envoy/network/transport_socket.h"

#include "extensions/transport_sockets/tap/tap_config.h"

namespace Envoy {
namespace Extensions {
namespace TransportSockets {
namespace Tap {

class TapSocket : public Network::TransportSocket {
public:
  TapSocket(PerSocketTapperPtr&& tapper, Network::TransportSocketPtr&& transport_socket,
            Event::TimeSystem& time_system);

  // Network::TransportSocket
  void setTransportSocketCallbacks(Network::TransportSocketCallbacks& callbacks) override;
  std::string protocol() const override;
  bool canFlushClose() override;
  void closeSocket(Network::ConnectionEvent event) override;
  Network::IoResult doRead(Buffer::Instance& buffer) override;
  Network::IoResult doWrite(Buffer::Instance& buffer, bool end_stream) override;
  void onConnected() override;
  const Ssl::Connection* ssl() const override;

private:
  // fixfixconst std::string& path_prefix_;
  // fixfixconst envoy::config::transport_socket::tap::v2alpha::FileSink::Format format_;
  PerSocketTapperPtr tapper_;
  // TODO(htuch): Buffering the entire trace until socket close won't scale to
  // long lived connections or large transfers. We could emit multiple tap
  // files with bounded size, with identical connection ID to allow later
  // reassembly.
  envoy::data::tap::v2alpha::Trace trace_;
  Network::TransportSocketPtr transport_socket_;
  Network::TransportSocketCallbacks* callbacks_{};
  Event::TimeSystem& time_system_;
};

class TapSocketFactory : public Network::TransportSocketFactory {
public:
  TapSocketFactory(SocketTapConfigFactoryPtr&& config_factory,
                   Network::TransportSocketFactoryPtr&& transport_socket_factory,
                   Event::TimeSystem& time_system);

  // Network::TransportSocketFactory
  Network::TransportSocketPtr
  createTransportSocket(Network::TransportSocketOptionsSharedPtr options) const override;
  bool implementsSecureTransport() const override;

private:
  // fixfixconst std::string path_prefix_;
  // fixfixconst envoy::config::transport_socket::tap::v2alpha::FileSink::Format format_;
  SocketTapConfigFactoryPtr config_factory_;
  Network::TransportSocketFactoryPtr transport_socket_factory_;
  Event::TimeSystem& time_system_;
};

} // namespace Tap
} // namespace TransportSockets
} // namespace Extensions
} // namespace Envoy
