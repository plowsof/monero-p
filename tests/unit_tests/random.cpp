// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

// 

#include "gtest/gtest.h"

#include "crypto/crypto.h"

extern "C" {
#include "crypto/crypto-ops.h"
}

TEST(random32_unbiased, less_than_order)
{
  unsigned char tmp[32], tmp2[32];
  for (int i = 0; i < 1000; ++i)
  {
    crypto::random32_unbiased(tmp);
    memcpy(tmp2, tmp, 32);
    sc_reduce32(tmp2);
    ASSERT_EQ(memcmp(tmp, tmp2, 32), 0);
  }
}
