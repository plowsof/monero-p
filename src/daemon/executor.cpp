// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 

#include "misc_log_ex.h"

#include "daemon/executor.h"

#include "cryptonote_config.h"
#include "version.h"

#include <string>

#undef MONERO_DEFAULT_LOG_CATEGORY
#define MONERO_DEFAULT_LOG_CATEGORY "daemon"

namespace daemonize
{
  std::string const t_executor::NAME = "Monero Daemon";

  void t_executor::init_options(
      boost::program_options::options_description & configurable_options
    )
  {
    t_daemon::init_options(configurable_options);
  }

  std::string const & t_executor::name()
  {
    return NAME;
  }

  t_daemon t_executor::create_daemon(
      boost::program_options::variables_map const & vm
    )
  {
    LOG_PRINT_L0("Monero '" << MONERO_RELEASE_NAME << "' (v" << MONERO_VERSION_FULL << ") Daemonised");
    return t_daemon{vm, public_rpc_port};
  }

  bool t_executor::run_non_interactive(
      boost::program_options::variables_map const & vm
    )
  {
    return t_daemon{vm, public_rpc_port}.run(false);
  }

  bool t_executor::run_interactive(
      boost::program_options::variables_map const & vm
    )
  {
    return t_daemon{vm, public_rpc_port}.run(true);
  }
}

