// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 
// 
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#pragma once

#include <string.h>
#include <sodium/crypto_verify_32.h>

struct memcmp32
{
  static const size_t loop_count = 10000000;
  static int call(const unsigned char *k0, const unsigned char *k1){ return memcmp(k0, k1, 32); }
};

struct verify32
{
  static const size_t loop_count = 10000000;
  static int call(const unsigned char *k0, const unsigned char *k1){ return crypto_verify_32(k0, k1); }
};

template<typename f, bool equal>
class test_equality
{
public:
  static const size_t loop_count = f::loop_count;

  bool init()
  {
    for (int n = 0; n < 32; ++n)
      k0[n] = n;
    for (int n = 0; n < 32; ++n)
      k1[n] = equal ? n : n + 1;
    return true;
  }

  bool test()
  {
    return equal == !f::call(k0, k1);
  }

private:
  unsigned char k0[32];
  unsigned char k1[32];
};

