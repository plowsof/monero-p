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

class test_derive_secret_key : public single_tx_test_base
{
public:
  static const size_t loop_count = 10000;

  bool init()
  {
    if (!single_tx_test_base::init())
      return false;

    crypto::generate_key_derivation(m_tx_pub_key, m_bob.get_keys().m_view_secret_key, m_key_derivation);
    m_spend_secret_key = m_bob.get_keys().m_spend_secret_key;

    return true;
  }

  bool test()
  {
    cryptonote::keypair in_ephemeral;
    crypto::derive_secret_key(m_key_derivation, 0, m_spend_secret_key, in_ephemeral.sec);
    return true;
  }

private:
  crypto::key_derivation m_key_derivation;
  crypto::secret_key m_spend_secret_key;
};
