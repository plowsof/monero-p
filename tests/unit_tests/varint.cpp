// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 
// 
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#include <cstring>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <vector>
#include <boost/foreach.hpp>
#include "cryptonote_basic/cryptonote_basic.h"
#include "cryptonote_basic/cryptonote_basic_impl.h"
#include "serialization/binary_archive.h"
#include "serialization/json_archive.h"
#include "serialization/debug_archive.h"
#include "serialization/variant.h"
#include "serialization/containers.h"
#include "serialization/binary_utils.h"
#include "gtest/gtest.h"
using namespace std;

TEST(varint, equal)
{
  for (uint64_t idx = 0; idx < 65537; ++idx)
  {
    char buf[12];
    char *bufptr = buf;
    tools::write_varint(bufptr, idx);
    uint64_t bytes = bufptr - buf;
    ASSERT_TRUE (bytes > 0 && bytes <= sizeof(buf));

    uint64_t idx2;
    std::string s(buf, bytes);
    int read = tools::read_varint(s.begin(), s.end(), idx2);
    ASSERT_EQ (read, bytes);
    ASSERT_TRUE(idx2 == idx);
  }
}
