// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 

#include "gtest/gtest.h"
#include "int-util.h"
#include "cryptonote_basic/difficulty.h"

static cryptonote::difficulty_type MKDIFF(uint64_t high, uint64_t low)
{
  cryptonote::difficulty_type d = high;
  d = (d << 64) | low;
  return d;
}

static crypto::hash MKHASH(uint64_t high, uint64_t low)
{
  cryptonote::difficulty_type hash_target = high;
  hash_target = (hash_target << 64) | low;
  boost::multiprecision::uint256_t hash_value = std::numeric_limits<boost::multiprecision::uint256_t>::max() / hash_target;
  crypto::hash h;
  uint64_t val;
  val = (hash_value & 0xffffffffffffffff).convert_to<uint64_t>();
  ((uint64_t*)&h)[0] = SWAP64LE(val);
  hash_value >>= 64;
  val = (hash_value & 0xffffffffffffffff).convert_to<uint64_t>();
  ((uint64_t*)&h)[1] = SWAP64LE(val);
  hash_value >>= 64;
  val = (hash_value & 0xffffffffffffffff).convert_to<uint64_t>();
  ((uint64_t*)&h)[2] = SWAP64LE(val);
  hash_value >>= 64;
  val = (hash_value & 0xffffffffffffffff).convert_to<uint64_t>();
  ((uint64_t*)&h)[3] = SWAP64LE(val);
  return h;
}

TEST(difficulty, check_hash)
{
  ASSERT_TRUE(cryptonote::check_hash(MKHASH(0, 1), MKDIFF(0, 1)));
  ASSERT_FALSE(cryptonote::check_hash(MKHASH(0, 1), MKDIFF(0, 2)));

  ASSERT_TRUE(cryptonote::check_hash(MKHASH(0, 0xffffffffffffffff), MKDIFF(0, 0xffffffffffffffff)));
  ASSERT_FALSE(cryptonote::check_hash(MKHASH(0, 0xffffffffffffffff), MKDIFF(1, 0)));

  ASSERT_TRUE(cryptonote::check_hash(MKHASH(1, 1), MKDIFF(1, 1)));
  ASSERT_FALSE(cryptonote::check_hash(MKHASH(1, 1), MKDIFF(1, 2)));

  ASSERT_TRUE(cryptonote::check_hash(MKHASH(0xffffffffffffffff, 1), MKDIFF(0xffffffffffffffff, 1)));
  ASSERT_FALSE(cryptonote::check_hash(MKHASH(0xffffffffffffffff, 1), MKDIFF(0xffffffffffffffff, 2)));
}
