// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 

#pragma once

#include "cryptonote_basic/cryptonote_format_utils.h"

class test_is_valid_decomposed_amount
{
public:
  static const size_t loop_count = 3;
  static const uint64_t max_per_loop = 1000000000; // must be power of 10

  bool init()
  {
    m_num_valid_per_loop = 1;
    uint64_t x = 1;
    while (x != max_per_loop)
    {
        m_num_valid_per_loop += 9;
        x *= 10;
    }

    return true;
  }

  bool test()
  {
    size_t num_valid = 0;
    for (uint64_t a = 0; a <= max_per_loop; ++a)
        num_valid += cryptonote::is_valid_decomposed_amount(a);
    return num_valid == m_num_valid_per_loop;
  }

  size_t m_num_valid_per_loop;
};
