// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 
// 
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#pragma once

#include "chaingen.h"
#include "block_reward.h"
#include "block_validation.h"
#include "chain_split_1.h"
#include "chain_switch_1.h"
#include "double_spend.h"
#include "integer_overflow.h"
#include "ring_signature_1.h"
#include "tx_validation.h"
#include "v2_tests.h"
#include "rct.h"
#include "multisig.h"
#include "bulletproofs.h"
#include "bulletproof_plus.h"
#include "rct2.h"
/************************************************************************/
/*                                                                      */
/************************************************************************/
class gen_simple_chain_001: public test_chain_unit_base 
{
public: 
  gen_simple_chain_001();
  bool generate(std::vector<test_event_entry> &events);
  bool verify_callback_1(cryptonote::core& c, size_t ev_index, const std::vector<test_event_entry> &events); 
  bool verify_callback_2(cryptonote::core& c, size_t ev_index, const std::vector<test_event_entry> &events); 
};

class one_block: public test_chain_unit_base
{
  cryptonote::account_base alice;
public:
  one_block();
  bool generate(std::vector<test_event_entry> &events);
  bool verify_1(cryptonote::core& c, size_t ev_index, const std::vector<test_event_entry> &events);
};
