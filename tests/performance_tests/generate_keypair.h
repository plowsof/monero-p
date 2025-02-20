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

class test_generate_keypair
{
public:
  static const size_t loop_count = 10000;

  bool init()
  {
    return true;
  }

  bool test()
  {
    cryptonote::keypair::generate(hw::get_device("default"));
    return true;
  }
};
