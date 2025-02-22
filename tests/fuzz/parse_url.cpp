// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 

#include "include_base_utils.h"
#include "file_io_utils.h"
#include "net/net_parse_helpers.h"
#include "fuzzer.h"

BEGIN_INIT_SIMPLE_FUZZER()
END_INIT_SIMPLE_FUZZER()

BEGIN_SIMPLE_FUZZER()
  epee::net_utils::http::url_content url;
  epee::net_utils::parse_url(std::string((const char*)buf, len), url);
END_SIMPLE_FUZZER()
