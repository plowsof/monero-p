// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 
// 
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#pragma once

#include "ringct/rctSigs.h"
#include "ringct/bulletproofs_plus.h"

template<bool a_verify, size_t n_amounts>
class test_bulletproof_plus
{
public:
  static const size_t approx_loop_count = 100 / n_amounts;
  static const size_t loop_count = (approx_loop_count >= 10 ? approx_loop_count : 10) / (a_verify ? 1 : 5);
  static const bool verify = a_verify;

  bool init()
  {
    proof = rct::bulletproof_plus_PROVE(std::vector<uint64_t>(n_amounts, 749327532984), rct::skvGen(n_amounts));
    return true;
  }

  bool test()
  {
    bool ret = true;
    if (verify)
      ret = rct::bulletproof_plus_VERIFY(proof);
    else
      rct::bulletproof_plus_PROVE(std::vector<uint64_t>(n_amounts, 749327532984), rct::skvGen(n_amounts));
    return ret;
  }

private:
  rct::BulletproofPlus proof;
};

template<bool batch, size_t start, size_t repeat, size_t mul, size_t add, size_t N>
class test_aggregated_bulletproof_plus
{
public:
  static const size_t loop_count = 500 / (N * repeat);

  bool init()
  {
    size_t o = start;
    for (size_t n = 0; n < N; ++n)
    {
      for (size_t i = 0; i < repeat; ++i)
        proofs.push_back(rct::bulletproof_plus_PROVE(std::vector<uint64_t>(o, 749327532984), rct::skvGen(o)));
      o = o * mul + add;
    }
    return true;
  }

  bool test()
  {
    if (batch)
    {
        return rct::bulletproof_plus_VERIFY(proofs);
    }
    else
    {
      for (const rct::BulletproofPlus &proof: proofs)
        if (!rct::bulletproof_plus_VERIFY(proof))
          return false;
      return true;
    }
  }

private:
  std::vector<rct::BulletproofPlus> proofs;
};
