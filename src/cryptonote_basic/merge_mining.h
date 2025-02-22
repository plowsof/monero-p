// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

// 

#pragma once

#include <stdint.h>
#include "crypto/crypto.h"

namespace cryptonote
{
  uint32_t get_aux_slot(const crypto::hash &id, uint32_t nonce, uint32_t n_aux_chains);
  uint32_t get_path_from_aux_slot(uint32_t slot, uint32_t n_aux_chains);
  uint64_t encode_mm_depth(uint32_t n_aux_chains, uint32_t nonce);
  bool decode_mm_depth(uint64_t depth, uint32_t &n_aux_chains, uint32_t &nonce);
}
