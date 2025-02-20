// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 

#include <set>
#include "cryptonote_basic/blobdatatype.h"

namespace cryptonote
{
  bool tx_sanity_check(const cryptonote::blobdata &tx_blob, uint64_t rct_outs_available);
  bool tx_sanity_check(const std::set<uint64_t> &rct_indices, size_t n_indices, uint64_t rct_outs_available);
}
