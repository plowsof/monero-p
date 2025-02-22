// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 
// 
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <boost/multiprecision/cpp_int.hpp>
#include "crypto/hash.h"

namespace cryptonote
{
    typedef boost::multiprecision::uint128_t difficulty_type;

    /**
     * @brief checks if a hash fits the given difficulty
     *
     * The hash passes if (hash * difficulty) < 2^256.
     * Phrased differently, if (hash * difficulty) fits without overflow into
     * the least significant 256 bits of the 320 bit multiplication result.
     *
     * @param hash the hash to check
     * @param difficulty the difficulty to check against
     *
     * @return true if valid, else false
     */
    bool check_hash_64(const crypto::hash &hash, uint64_t difficulty);
    uint64_t next_difficulty_64(std::vector<std::uint64_t> timestamps, std::vector<uint64_t> cumulative_difficulties, size_t target_seconds);

    bool check_hash_128(const crypto::hash &hash, difficulty_type difficulty);
    bool check_hash(const crypto::hash &hash, difficulty_type difficulty);
    difficulty_type next_difficulty(std::vector<std::uint64_t> timestamps, std::vector<difficulty_type> cumulative_difficulties, size_t target_seconds);

    std::string hex(difficulty_type v);
}
