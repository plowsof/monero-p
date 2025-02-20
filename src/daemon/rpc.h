// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 
//
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#pragma once

#include "rpc/core_rpc_server.h"

#undef MONERO_DEFAULT_LOG_CATEGORY
#define MONERO_DEFAULT_LOG_CATEGORY "daemon"

namespace daemonize
{

class t_rpc final
{
public:
  static void init_options(boost::program_options::options_description & option_spec)
  {
    cryptonote::core_rpc_server::init_options(option_spec);
  }
private:
  cryptonote::core_rpc_server m_server;
  const std::string m_description;
public:
  t_rpc(
      boost::program_options::variables_map const & vm
    , t_core & core
    , t_p2p & p2p
    , const bool restricted
    , const std::string & port
    , const std::string & description
    , bool allow_rpc_payment
    )
    : m_server{core.get(), p2p.get()}, m_description{description}
  {
    MGINFO("Initializing " << m_description << " RPC server...");

    if (!m_server.init(vm, restricted, port, allow_rpc_payment, command_line::get_arg(vm, daemon_args::arg_proxy)))
    {
      throw std::runtime_error("Failed to initialize " + m_description + " RPC server.");
    }
    MGINFO(m_description << " RPC server initialized OK on port: " << m_server.get_binded_port());
  }

  void run()
  {
    MGINFO("Starting " << m_description << " RPC server...");
    if (!m_server.run(2, false))
    {
      throw std::runtime_error("Failed to start " + m_description + " RPC server.");
    }
    MGINFO(m_description << " RPC server started ok");
  }

  void stop()
  {
    MGINFO("Stopping " << m_description << " RPC server...");
    m_server.send_stop_signal();
    m_server.timed_wait_server_stop(5000);
  }

  cryptonote::core_rpc_server* get_server()
  {
    return &m_server;
  }

  ~t_rpc()
  {
    MGINFO("Deinitializing " << m_description << " RPC server...");
    try {
      m_server.deinit();
    } catch (...) {
      MERROR("Failed to deinitialize " << m_description << " RPC server...");
    }
  }
};

}
