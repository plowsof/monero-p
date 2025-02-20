// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 

#pragma once

#include "string_tools.h"
#include "int-util.h"
#include "cryptonote_basic/difficulty.h"

template<uint64_t hash_target_high, uint64_t hash_target_low, uint64_t difficulty_high, uint64_t difficulty_low>
class test_check_hash
{
public:
  static const size_t loop_count = 100000;

  bool init()
  {
    cryptonote::difficulty_type hash_target = hash_target_high;
    hash_target = (hash_target << 64) | hash_target_low;
    difficulty = difficulty_high;
    difficulty = (difficulty << 64) | difficulty_low;
    boost::multiprecision::uint256_t hash_value =  std::numeric_limits<boost::multiprecision::uint256_t>::max() / hash_target;
    uint64_t val;
    val = (hash_value & 0xffffffffffffffff).convert_to<uint64_t>();
    ((uint64_t*)&hash)[0] = SWAP64LE(val);
    hash_value >>= 64;
    val = (hash_value & 0xffffffffffffffff).convert_to<uint64_t>();
    ((uint64_t*)&hash)[1] = SWAP64LE(val);
    hash_value >>= 64;
    val = (hash_value & 0xffffffffffffffff).convert_to<uint64_t>();
    ((uint64_t*)&hash)[2] = SWAP64LE(val);
    hash_value >>= 64;
    val = (hash_value & 0xffffffffffffffff).convert_to<uint64_t>();
    ((uint64_t*)&hash)[3] = SWAP64LE(val);
    return true;
  }

  bool test()
  {
    cryptonote::check_hash_128(hash, difficulty);
    return true;
  }

private:
  crypto::hash hash;
  cryptonote::difficulty_type difficulty;
};
