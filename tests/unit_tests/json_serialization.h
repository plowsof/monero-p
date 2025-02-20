// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

//

#pragma once

namespace test
{
  cryptonote::transaction make_miner_transaction(cryptonote::account_public_address const& to);

  cryptonote::transaction
  make_transaction(
      cryptonote::account_keys const& from,
      std::vector<cryptonote::transaction> const& sources,
      std::vector<cryptonote::account_public_address> const& destinations,
      bool rct,
      bool bulletproof);
}
