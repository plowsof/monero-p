// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 

#pragma once

#include <boost/thread/thread.hpp>
#include <boost/utility/string_ref.hpp>
#include <cstdint>
#include <memory>
#include <string>

#include "common/command_line.h"
#include "cryptonote_basic/fwd.h"
#include "net/zmq.h"
#include "rpc/fwd.h"
#include "rpc/rpc_handler.h"
#include "span.h"

namespace cryptonote
{

namespace rpc
{

class ZmqServer final
{
  public:

    ZmqServer(RpcHandler& h);

    ~ZmqServer();

    void serve();

    //! \return ZMQ context on success, `nullptr` on failure
    void* init_rpc(boost::string_ref address, boost::string_ref port);

    //! \return `nullptr` on errors.
    std::shared_ptr<listener::zmq_pub> init_pub(epee::span<const std::string> addresses);

    void run();
    void stop();

  private:
    RpcHandler& handler;

    net::zmq::context context;

    boost::thread run_thread;

    net::zmq::socket rep_socket;
    net::zmq::socket pub_socket;
    net::zmq::socket relay_socket;
    std::shared_ptr<listener::zmq_pub> shared_state;
};

}  // namespace cryptonote

}  // namespace rpc
