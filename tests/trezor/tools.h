// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
//

#pragma once

#include "misc_log_ex.h"
#include "daemon/daemon.h"
#include "rpc/daemon_handler.h"
#include "rpc/zmq_server.h"
#include "common/password.h"
#include "common/util.h"
#include "daemon/core.h"
#include "daemon/p2p.h"
#include "daemon/protocol.h"
#include "daemon/rpc.h"
#include "daemon/command_server.h"
#include "daemon/command_server.h"
#include "daemon/command_line_args.h"
#include "version.h"

namespace tools {

class options {
public:
  static void set_option(boost::program_options::variables_map &vm, const std::string &key, const boost::program_options::variable_value &pv);
  static void build_options(boost::program_options::variables_map & vm, const boost::program_options::options_description & desc_params);

  template<typename T, bool required, bool dependent, int NUM_DEPS>
  static void set_option(boost::program_options::variables_map &vm, const command_line::arg_descriptor<T, required, dependent, NUM_DEPS> &arg, const boost::program_options::variable_value &pv)
  {
    set_option(vm, arg.name, pv);
  }
};

};

