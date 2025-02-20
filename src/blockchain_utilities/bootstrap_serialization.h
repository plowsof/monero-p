// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
//

#pragma once

#include "cryptonote_basic/cryptonote_boost_serialization.h"
#include "serialization/difficulty_type.h"


namespace cryptonote
{
  namespace bootstrap
  {

    struct file_info
    {
      uint8_t  major_version;
      uint8_t  minor_version;
      uint32_t header_size;

      BEGIN_SERIALIZE_OBJECT()
        FIELD(major_version);
        FIELD(minor_version);
        VARINT_FIELD(header_size);
      END_SERIALIZE()
    };

    struct blocks_info
    {
      // block heights of file's first and last blocks, zero-based indexes
      uint64_t block_first;
      uint64_t block_last;

      // file position, for directly reading last block
      uint64_t block_last_pos;

      BEGIN_SERIALIZE_OBJECT()
        VARINT_FIELD(block_first);
        VARINT_FIELD(block_last);
        VARINT_FIELD(block_last_pos);
      END_SERIALIZE()
    };

    struct block_package_1
    {
      cryptonote::block block;
      std::vector<transaction> txs;
      size_t block_weight;
      uint64_t cumulative_difficulty;
      uint64_t coins_generated;

      BEGIN_SERIALIZE()
        FIELD(block)
        FIELD(txs)
        VARINT_FIELD(block_weight)
        VARINT_FIELD(cumulative_difficulty)
        VARINT_FIELD(coins_generated)
      END_SERIALIZE()
    };

    struct block_package
    {
      cryptonote::block block;
      std::vector<transaction> txs;
      size_t block_weight;
      difficulty_type cumulative_difficulty;
      uint64_t coins_generated;

      BEGIN_SERIALIZE()
        FIELD(block)
        FIELD(txs)
        VARINT_FIELD(block_weight)
        FIELD(cumulative_difficulty)
        VARINT_FIELD(coins_generated)
      END_SERIALIZE()
    };

  }

}
