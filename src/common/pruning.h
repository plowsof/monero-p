// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

// 

#pragma once

#include <stdint.h>

namespace tools
{
  static constexpr uint32_t PRUNING_SEED_LOG_STRIPES_SHIFT = 7;
  static constexpr uint32_t PRUNING_SEED_LOG_STRIPES_MASK = 0x7;
  static constexpr uint32_t PRUNING_SEED_STRIPE_SHIFT = 0;
  static constexpr uint32_t PRUNING_SEED_STRIPE_MASK = 0x7f;

  constexpr inline uint32_t get_pruning_log_stripes(uint32_t pruning_seed) { return (pruning_seed >> PRUNING_SEED_LOG_STRIPES_SHIFT) & PRUNING_SEED_LOG_STRIPES_MASK; }
  inline uint32_t get_pruning_stripe(uint32_t pruning_seed) { if (pruning_seed == 0) return 0; return 1 + ((pruning_seed >> PRUNING_SEED_STRIPE_SHIFT) & PRUNING_SEED_STRIPE_MASK); }

  uint32_t make_pruning_seed(uint32_t stripe, uint32_t log_stripes);

  bool has_unpruned_block(uint64_t block_height, uint64_t blockchain_height, uint32_t pruning_seed);
  uint32_t get_pruning_stripe(uint64_t block_height, uint64_t blockchain_height, uint32_t log_stripes);
  uint32_t get_pruning_seed(uint64_t block_height, uint64_t blockchain_height, uint32_t log_stripes);
  uint64_t get_next_unpruned_block_height(uint64_t block_height, uint64_t blockchain_height, uint32_t pruning_seed);
  uint64_t get_next_pruned_block_height(uint64_t block_height, uint64_t blockchain_height, uint32_t pruning_seed);
  uint32_t get_random_stripe();
}

