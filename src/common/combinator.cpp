// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

//
//
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#include "combinator.h"

namespace tools {

uint64_t combinations_count(uint32_t k, uint32_t n)
{
  if (k > n) {
    throw std::runtime_error("k must not be greater than n");
  }

  uint64_t c = 1;
  for (uint64_t i = 1; i <= k; ++i) {
    c *= n--;
    c /= i;
  }

  return c;
}

}
