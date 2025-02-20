// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 

#include "include_base_utils.h"
#include "common/base58.h"
#include "fuzzer.h"

BEGIN_INIT_SIMPLE_FUZZER()
END_INIT_SIMPLE_FUZZER()

BEGIN_SIMPLE_FUZZER()
  std::string data;
  tools::base58::decode(std::string((const char*)buf, len), data);
END_SIMPLE_FUZZER()
