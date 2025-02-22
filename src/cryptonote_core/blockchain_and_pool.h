// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
//

#pragma once

#include <memory>

#include "blockchain.h"
#include "tx_pool.h"
#include "warnings.h"

namespace cryptonote
{
/**
 * @brief Container for safely constructing Blockchain and tx_memory_pool classes
 * 
 * The reason for this class existing is that the constructors for both Blockchain and
 * tx_memory_pool take a reference for tx_memory_pool and Blockchain, respectively. Because of this
 * circular reference, it is annoying/unsafe to construct these normally. This class guarantees that
 * we don't make any silly mistakes with pointers / dangling references.
 */
struct BlockchainAndPool
{
    Blockchain blockchain;
    tx_memory_pool tx_pool;

PUSH_WARNINGS
DISABLE_GCC_WARNING(uninitialized)
    BlockchainAndPool(): blockchain(tx_pool), tx_pool(blockchain) {}
POP_WARNINGS
};
}
