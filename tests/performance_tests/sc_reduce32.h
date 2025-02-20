// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 

#pragma once

#include "crypto/crypto.h"

class test_sc_reduce32
{
public:
  static const size_t loop_count = 10000000;

  bool init()
  {
    m_hash = crypto::rand<crypto::hash>();
    return true;
  }

  bool test()
  {
    crypto::hash reduced = m_hash;
    sc_reduce32((unsigned char*)reduced.data);
    return true;
  }

private:
  crypto::hash m_hash;
};
