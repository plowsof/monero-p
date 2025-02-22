// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

// 

#pragma once

#include "crypto/crypto.h"

class test_sc_check
{
public:
  static const size_t loop_count = 10000000;

  bool init()
  {
    m_scalar = crypto::rand<crypto::ec_scalar>();
    return true;
  }

  bool test()
  {
    sc_check((unsigned char*)m_scalar.data);
    return true;
  }

private:
  crypto::ec_scalar m_scalar;
};
