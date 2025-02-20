// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
//

#include "tools.h"

namespace tools {

namespace po = boost::program_options;

void options::set_option(boost::program_options::variables_map &vm, const std::string & key, const po::variable_value &pv)
{
  auto it = vm.find(key);
  if (it == vm.end())
  {
    vm.insert(std::make_pair(key, pv));
  }
  else
    {
    it->second = pv;
  }
}

void options::build_options(boost::program_options::variables_map & vm, const po::options_description & desc_params)
{
  const char *argv[2] = {nullptr};
  po::store(po::parse_command_line(1, argv, desc_params), vm);
  po::notify(vm);
}

}

