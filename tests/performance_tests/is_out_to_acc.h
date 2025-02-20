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

#include "single_tx_test_base.h"

class test_is_out_to_acc : public single_tx_test_base
{
public:
  static const size_t loop_count = 1000;

  bool test()
  {
    crypto::public_key output_public_key;
    cryptonote::get_output_public_key(m_tx.vout[0], output_public_key);
    return cryptonote::is_out_to_acc(m_bob.get_keys(), output_public_key, m_tx_pub_key, m_additional_tx_pub_keys, 0);
  }
};

class test_is_out_to_acc_precomp : public single_tx_test_base
{
public:
  static const size_t loop_count = 1000;

  bool init()
  {
    if (!single_tx_test_base::init())
      return false;
    crypto::generate_key_derivation(m_tx_pub_key, m_bob.get_keys().m_view_secret_key, m_derivation);
    return true;
  }
  bool test()
  {
    const cryptonote::txout_to_key& tx_out = boost::get<cryptonote::txout_to_key>(m_tx.vout[0].target);
    std::unordered_map<crypto::public_key, cryptonote::subaddress_index> subaddresses;
    subaddresses[m_bob.get_keys().m_account_address.m_spend_public_key] = {0,0};
    std::vector<crypto::key_derivation> additional_derivations;
    boost::optional<cryptonote::subaddress_receive_info> info = cryptonote::is_out_to_acc_precomp(subaddresses, tx_out.key, m_derivation, additional_derivations, 0, hw::get_device("default"));
    return (bool)info;
  }

private:
  crypto::key_derivation m_derivation;
};
