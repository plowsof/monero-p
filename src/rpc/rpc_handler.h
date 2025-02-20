// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 

#pragma once

#include <boost/optional/optional.hpp>
#include <cstdint>
#include <string>
#include <vector>
#include "byte_slice.h"
#include "crypto/hash.h"

namespace cryptonote
{
class core;

namespace rpc
{

struct output_distribution_data
{
  std::vector<std::uint64_t> distribution;
  std::uint64_t start_height;
  std::uint64_t base;
};

class RpcHandler
{
  public:
    RpcHandler() { }
    virtual ~RpcHandler() { }

    virtual epee::byte_slice handle(std::string&& request) = 0;

    static boost::optional<output_distribution_data>
      get_output_distribution(const std::function<bool(uint64_t, uint64_t, uint64_t, uint64_t&, std::vector<uint64_t>&, uint64_t&)> &f, uint64_t amount, uint64_t from_height, uint64_t to_height, const std::function<crypto::hash(uint64_t)> &get_hash, bool cumulative, uint64_t blockchain_height);
};


}  // rpc

}  // cryptonote
