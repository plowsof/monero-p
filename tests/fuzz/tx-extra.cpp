// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

// 

#include "include_base_utils.h"
#include "cryptonote_basic/cryptonote_format_utils.h"
#include "fuzzer.h"

BEGIN_INIT_SIMPLE_FUZZER()
END_INIT_SIMPLE_FUZZER()

BEGIN_SIMPLE_FUZZER()
  std::vector<cryptonote::tx_extra_field> tx_extra_fields;
  cryptonote::parse_tx_extra(std::vector<uint8_t>(buf, buf + len), tx_extra_fields);
END_SIMPLE_FUZZER()

