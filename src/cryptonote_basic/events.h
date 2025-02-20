// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

//

#pragma once

#include "crypto/hash.h"
#include "cryptonote_basic/cryptonote_basic.h"

namespace cryptonote
{
  /*! Transactions are expensive to move or copy (lots of 32-byte internal
      buffers). This allows `cryptonote::core` to do a single notification for
      a vector of transactions, without having to move/copy duplicate or invalid
      transactions. */
  struct txpool_event
  {
    cryptonote::transaction tx;
    crypto::hash hash;
    uint64_t blob_size;
    uint64_t weight;
    bool res; //!< Listeners must ignore `tx` when this is false.
  };
}
