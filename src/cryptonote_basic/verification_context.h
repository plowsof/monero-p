// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 
// 
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#pragma once

#include "cryptonote_protocol/enums.h"

namespace cryptonote
{
  /************************************************************************/
  /*                                                                      */
  /************************************************************************/
  struct tx_verification_context
  {
    static_assert(unsigned(relay_method::none) == 0, "default m_relay initialization is not to relay_method::none");

    relay_method m_relay; // gives indication on how tx should be relayed (if at all)
    bool m_verifivation_failed; //bad tx, tx should not enter mempool and connection should be dropped unless m_no_drop_offense
    // Do not add to mempool, do not relay, but also do not punish the peer for sending or drop
    // connections to them. Used for low fees, tx_extra too big, "relay-only rules". Not to be
    // confused with breaking soft fork rules, because tx could be later added to the chain if mined
    // because it does not violate consensus rules.
    bool m_no_drop_offense;
    bool m_verifivation_impossible; //the transaction is related with an alternative blockchain
    bool m_added_to_pool; 
    bool m_low_mixin;
    bool m_double_spend;
    bool m_invalid_input;
    bool m_invalid_output;
    bool m_too_big;
    bool m_overspend;
    bool m_fee_too_low;
    bool m_too_few_outputs;
    bool m_tx_extra_too_big;
    bool m_nonzero_unlock_time;
  };

  struct block_verification_context
  {
    bool m_added_to_main_chain;
    bool m_verifivation_failed; //bad block, should drop connection
    bool m_marked_as_orphaned;
    bool m_already_exists;
    bool m_partial_block_reward;
    bool m_bad_pow; // if bad pow, bad peer outright for DoS protection
  };
}
