// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 

#pragma once
#include <boost/program_options.hpp>

#undef MONERO_DEFAULT_LOG_CATEGORY
#define MONERO_DEFAULT_LOG_CATEGORY "daemon"

namespace daemonize {

struct t_internals;

class t_daemon final {
public:
  static void init_options(boost::program_options::options_description & option_spec);
private:
  void stop_p2p();
private:
  std::unique_ptr<t_internals> mp_internals;
  uint16_t public_rpc_port;
public:
  t_daemon(
      boost::program_options::variables_map const & vm,
      uint16_t public_rpc_port = 0
    );
  t_daemon(t_daemon && other);
  t_daemon & operator=(t_daemon && other);
  ~t_daemon();

  bool run(bool interactive = false);
  void stop();
};
}
