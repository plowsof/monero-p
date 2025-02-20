// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 
// 
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#pragma once 
#include "chaingen.h"

struct gen_block_reward : public test_chain_unit_base
{
  gen_block_reward();

  bool generate(std::vector<test_event_entry>& events) const;

  bool check_block_verification_context(const cryptonote::block_verification_context& bvc, size_t event_idx, const cryptonote::block& blk);

  bool mark_invalid_block(cryptonote::core& c, size_t ev_index, const std::vector<test_event_entry>& events);
  bool mark_checked_block(cryptonote::core& c, size_t ev_index, const std::vector<test_event_entry>& events);
  bool check_block_rewards(cryptonote::core& c, size_t ev_index, const std::vector<test_event_entry>& events);

private:
  size_t m_invalid_block_index;
  std::vector<size_t> m_checked_blocks_indices;
};
