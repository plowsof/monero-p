// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 

#pragma once 

#include <string>

namespace tools
{
  bool check_updates(const std::string &software, const std::string &buildtag, std::string &version, std::string &hash);
  std::string get_update_url(const std::string &software, const std::string &subdir, const std::string &buildtag, const std::string &version, bool user);
}
