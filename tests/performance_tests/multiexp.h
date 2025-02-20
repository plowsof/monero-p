// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

// 
// 
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#pragma once

#include <vector>
#include "ringct/rctOps.h"
#include "ringct/multiexp.h"

enum test_multiexp_algorithm
{
  multiexp_bos_coster,
  multiexp_straus,
  multiexp_straus_cached,
  multiexp_pippenger,
  multiexp_pippenger_cached,
};

template<test_multiexp_algorithm algorithm, size_t npoints, size_t c=0>
class test_multiexp
{
public:
  static const size_t loop_count = npoints >= 1024 ? 10 : npoints < 256 ? 1000 : 100;

  bool init()
  {
    data.resize(npoints);
    res = rct::identity();
    for (size_t n = 0; n < npoints; ++n)
    {
      data[n].scalar = rct::skGen();
      rct::key point = rct::scalarmultBase(rct::skGen());
      if (ge_frombytes_vartime(&data[n].point, point.bytes))
        return false;
      rct::key kn = rct::scalarmultKey(point, data[n].scalar);
      res = rct::addKeys(res, kn);
    }
    straus_cache = rct::straus_init_cache(data);
    pippenger_cache = rct::pippenger_init_cache(data);
    return true;
  }

  bool test()
  {
    switch (algorithm)
    {
      case multiexp_bos_coster:
        return res == bos_coster_heap_conv_robust(data);
      case multiexp_straus:
        return res == straus(data);
      case multiexp_straus_cached:
        return res == straus(data, straus_cache);
      case multiexp_pippenger:
        return res == pippenger(data, NULL, 0, c);
      case multiexp_pippenger_cached:
        return res == pippenger(data, pippenger_cache, 0, c);
      default:
        return false;
    }
  }

private:
  std::vector<rct::MultiexpData> data;
  std::shared_ptr<rct::straus_cached_data> straus_cache;
  std::shared_ptr<rct::pippenger_cached_data> pippenger_cache;
  rct::key res;
};
