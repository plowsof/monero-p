// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 
// 
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#pragma once

#include "string_tools.h"
#include "crypto/crypto.h"
#include "cryptonote_basic/cryptonote_basic.h"

template<unsigned int variant>
class test_cn_slow_hash
{
public:
  static const size_t loop_count = 10;

#pragma pack(push, 1)
  struct data_t
  {
    char data[43];
  };
#pragma pack(pop)

  static_assert(43 == sizeof(data_t), "Invalid structure size");

  bool init()
  {
    if (!epee::string_tools::hex_to_pod("63617665617420656d70746f763617665617420656d70746f72263617665617420656d70746f7201020304", m_data))
      return false;

    return true;
  }

  bool test()
  {
    crypto::hash hash;
    crypto::cn_slow_hash(&m_data, sizeof(m_data), hash, variant);
    return true;
  }

private:
  data_t m_data;
};
