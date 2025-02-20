// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

// 

#pragma once 

#include <string>
#include <vector>

namespace tools
{

class Notify
{
public:
  Notify(const char *spec);
  Notify(const Notify&) = default;
  Notify(Notify&&) = default;
  Notify& operator=(const Notify&) = default;
  Notify& operator=(Notify&&) = default;

  int notify(const char *tag, const char *s, ...) const;

private:
  std::string filename;
  std::vector<std::string> args;
};

}

