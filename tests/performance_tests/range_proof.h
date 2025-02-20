// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 
// 
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#pragma once

#include "ringct/rctSigs.h"

template<bool a_verify>
class test_range_proof
{
public:
  static const size_t loop_count = 50;
  static const bool verify = a_verify;

  bool init()
  {
    rct::key mask;
    sig = rct::proveRange(C, mask, 84932483243793);
    return true;
  }

  bool test()
  {
    bool ret = true;
    rct::key mask;
    if (verify)
      ret = rct::verRange(C, sig);
    else
      rct::proveRange(C, mask, 84932483243793);
    return ret;
  }

private:
  rct::key C;
  rct::rangeSig sig;
};
