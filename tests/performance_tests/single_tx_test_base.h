// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 
// 
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#pragma once

#include "cryptonote_basic/account.h"
#include "cryptonote_basic/cryptonote_basic.h"
#include "cryptonote_core/cryptonote_tx_utils.h"

class single_tx_test_base
{
public:
  bool init()
  {
    using namespace cryptonote;

    m_bob.generate();

    if (!construct_miner_tx(0, 0, 0, 2, 0, m_bob.get_keys().m_account_address, m_tx))
      return false;

    m_tx_pub_key = get_tx_pub_key_from_extra(m_tx);
    m_additional_tx_pub_keys = get_additional_tx_pub_keys_from_extra(m_tx);
    return true;
  }

protected:
  cryptonote::account_base m_bob;
  cryptonote::transaction m_tx;
  crypto::public_key m_tx_pub_key;
  std::vector<crypto::public_key> m_additional_tx_pub_keys;
};
