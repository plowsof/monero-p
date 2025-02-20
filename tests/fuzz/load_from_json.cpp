// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 

#include "include_base_utils.h"
#include "file_io_utils.h"
#include "serialization/keyvalue_serialization.h"
#include "storages/portable_storage_template_helper.h"
#include "storages/portable_storage_base.h"
#include "fuzzer.h"

BEGIN_INIT_SIMPLE_FUZZER()
END_INIT_SIMPLE_FUZZER()

BEGIN_SIMPLE_FUZZER()
  epee::serialization::portable_storage ps;
  ps.load_from_json(std::string((const char*)buf, len));
END_SIMPLE_FUZZER()
