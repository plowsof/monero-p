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

class test_ge_frombytes_vartime : public multi_tx_test_base<1>
{
public:
  static const size_t loop_count = 10000;

  typedef multi_tx_test_base<1> base_class;

  bool init()
  {
    using namespace cryptonote;

    if (!base_class::init())
      return false;

    cryptonote::account_base m_alice;
    cryptonote::transaction m_tx;

    m_alice.generate();

    std::vector<tx_destination_entry> destinations;
    destinations.push_back(tx_destination_entry(1, m_alice.get_keys().m_account_address, false));

    if (!construct_tx(this->m_miners[this->real_source_idx].get_keys(), this->m_sources, destinations, boost::none, std::vector<uint8_t>(), m_tx))
      return false;

    const cryptonote::txin_to_key& txin = boost::get<cryptonote::txin_to_key>(m_tx.vin[0]);
    m_key = rct::ki2rct(txin.k_image);

    return true;
  }

  bool test()
  {
    ge_p3 unp;
    return ge_frombytes_vartime(&unp, (const unsigned char*) &m_key) == 0;
  }

private:
  rct::key m_key;
};
