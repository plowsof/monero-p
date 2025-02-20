// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

// 
//
// Adapted from Python code by Sarang Noether

#pragma once

#ifndef MULTIEXP_H
#define MULTIEXP_H

#include <vector>
extern "C"
{
#include "crypto/crypto-ops.h"
}
#include "crypto/crypto.h"
#include "rctTypes.h"
#include "misc_log_ex.h"

#include <boost/align/aligned_allocator.hpp>

namespace rct
{

struct MultiexpData {
  rct::key scalar;
  ge_p3 point;

  MultiexpData() {}
  MultiexpData(const rct::key &s, const ge_p3 &p): scalar(s), point(p) {}
  MultiexpData(const rct::key &s, const rct::key &p): scalar(s)
  {
    CHECK_AND_ASSERT_THROW_MES(ge_frombytes_vartime(&point, p.bytes) == 0, "ge_frombytes_vartime failed");
  }
};

struct straus_cached_data;
using pippenger_cached_data = std::vector<ge_cached, boost::alignment::aligned_allocator<ge_cached, 4096>>;

rct::key bos_coster_heap_conv(std::vector<MultiexpData> data);
rct::key bos_coster_heap_conv_robust(std::vector<MultiexpData> data);
std::shared_ptr<straus_cached_data> straus_init_cache(const std::vector<MultiexpData> &data, size_t N =0);
size_t straus_get_cache_size(const std::shared_ptr<straus_cached_data> &cache);
ge_p3 straus_p3(const std::vector<MultiexpData> &data, const std::shared_ptr<straus_cached_data> &cache = NULL, size_t STEP = 0);
rct::key straus(const std::vector<MultiexpData> &data, const std::shared_ptr<straus_cached_data> &cache = NULL, size_t STEP = 0);
std::shared_ptr<pippenger_cached_data> pippenger_init_cache(const std::vector<MultiexpData> &data, size_t start_offset = 0, size_t N =0);
size_t pippenger_get_cache_size(const std::shared_ptr<pippenger_cached_data> &cache);
size_t get_pippenger_c(size_t N);
ge_p3 pippenger_p3(const std::vector<MultiexpData> &data, const std::shared_ptr<pippenger_cached_data> &cache = NULL, size_t cache_size = 0, size_t c = 0);
rct::key pippenger(const std::vector<MultiexpData> &data, const std::shared_ptr<pippenger_cached_data> &cache = NULL, const size_t cache_size = 0, const size_t c = 0);

}

#endif
