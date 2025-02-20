// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 

#pragma once

#include "daemon/daemon.h"
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <string>

#undef MONERO_DEFAULT_LOG_CATEGORY
#define MONERO_DEFAULT_LOG_CATEGORY "daemon"

namespace daemonize
{
  class t_executor final
  {
  public:
    typedef ::daemonize::t_daemon t_daemon;

    static std::string const NAME;

    t_executor(uint16_t public_rpc_port = 0) : public_rpc_port(public_rpc_port)
    {
    }

    static void init_options(
        boost::program_options::options_description & configurable_options
      );

    std::string const & name();

    t_daemon create_daemon(
        boost::program_options::variables_map const & vm
      );

    bool run_non_interactive(
        boost::program_options::variables_map const & vm
      );

    bool run_interactive(
        boost::program_options::variables_map const & vm
      );

  private:
    uint16_t public_rpc_port;
  };
}
