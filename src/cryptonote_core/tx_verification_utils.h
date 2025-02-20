// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
//

#pragma once

#include "common/data_cache.h"
#include "cryptonote_basic/cryptonote_basic.h"

namespace cryptonote
{

// Modifying this value should not affect consensus. You can adjust it for performance needs
static constexpr const size_t RCT_VER_CACHE_SIZE = 8192;

using rct_ver_cache_t = ::tools::data_cache<::crypto::hash, RCT_VER_CACHE_SIZE>;

/**
 * @brief Cached version of rct::verRctNonSemanticsSimple
 *
 * This function will not affect how the transaction is serialized and it will never modify the
 * transaction prefix.
 *
 * The reference to tx is mutable since the transaction's ring signatures may be expanded by
 * Blockchain::expand_transaction_2. However, on cache hits, the transaction will not be
 * expanded. This means that the caller does not need to call expand_transaction_2 on this
 * transaction before passing it; the transaction will not successfully verify with "old" RCT data
 * if the transaction has been otherwise modified since the last verification.
 *
 * But, if cryptonote::get_transaction_hash(tx) returns a "stale" hash, this function is not
 * guaranteed to work. So make sure that the cryptonote::transaction passed has not had
 * modifications to it since the last time its hash was fetched without properly invalidating the
 * hashes.
 *
 * rct_type_to_cache can be any RCT version value as long as rct::verRctNonSemanticsSimple works for
 * this RCT version, but for most applications, it doesn't make sense to not make this version
 * the "current" RCT version (i.e. the version that transactions in the mempool are).
 *
 * @param tx transaction which contains RCT signature to verify
 * @param mix_ring mixring referenced by this tx. THIS DATA MUST BE PREVIOUSLY VALIDATED
 * @param cache saves tx+mixring hashes used to cache calls
 * @param rct_type_to_cache Only RCT sigs with version (e.g. RCTTypeBulletproofPlus) will be cached
 * @return true when verRctNonSemanticsSimple() w/ expanded tx.rct_signatures would return true
 * @return false when verRctNonSemanticsSimple() w/ expanded tx.rct_signatures would return false
 */
bool ver_rct_non_semantics_simple_cached
(
    transaction& tx,
    const rct::ctkeyM& mix_ring,
    rct_ver_cache_t& cache,
    std::uint8_t rct_type_to_cache
);

} // namespace cryptonote
