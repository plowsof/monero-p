// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
//

#pragma once

#include "cryptonote_basic/blobdatatype.h"
#include "cryptonote_protocol/enums.h"
#include "span.h"

namespace cryptonote
{
  struct i_core_events
  {
    virtual ~i_core_events() noexcept
    {}

    virtual uint64_t get_current_blockchain_height() const = 0;
    virtual bool is_synchronized() const = 0;
    virtual void on_transactions_relayed(epee::span<const cryptonote::blobdata> tx_blobs, relay_method tx_relay) = 0;
  };
}
