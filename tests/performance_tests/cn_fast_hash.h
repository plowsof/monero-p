// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 
// 
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#pragma once

#include "crypto/crypto.h"
#include "cryptonote_basic/cryptonote_basic.h"

template<size_t bytes>
class test_cn_fast_hash
{
public:
  static const size_t loop_count = bytes < 256 ? 100000 : bytes < 4096 ? 10000 : 1000;

  bool init()
  {
    crypto::rand(bytes, m_data.data());
    return true;
  }

  bool test()
  {
    crypto::hash hash;
    crypto::cn_fast_hash(&m_data, bytes, hash);
    return true;
  }

private:
  std::array<uint8_t, bytes> m_data;
};
