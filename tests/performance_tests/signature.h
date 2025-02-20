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

#include "single_tx_test_base.h"

template<bool verify>
class test_signature : public single_tx_test_base
{
public:
  static const size_t loop_count = 10000;

  bool init()
  {
    if (!single_tx_test_base::init())
      return false;

    message = crypto::rand<crypto::hash>();
    keys = cryptonote::keypair::generate(hw::get_device("default"));
    crypto::generate_signature(message, keys.pub, keys.sec, m_signature);

    return true;
  }

  bool test()
  {
    if (verify)
      return crypto::check_signature(message, keys.pub, m_signature);
    crypto::generate_signature(message, keys.pub, keys.sec, m_signature);
    return true;
  }

private:
  cryptonote::keypair keys;
  crypto::hash message;
  crypto::signature m_signature;
};
