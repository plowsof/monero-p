// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

// 

#include "gtest/gtest.h"
#include "misc_log_ex.h"
#include "rpc/rpc_handler.h"
#include "cryptonote_core/cryptonote_core.h"
#include "blockchain_db/testdb.h"

static const uint64_t test_distribution[32] = {
  0, 0, 0, 0, 0, 1, 5, 1, 4, 0, 0, 1, 0, 1, 2, 3, 1, 0, 2, 0, 1, 3, 8, 1, 3, 5, 7, 1, 5, 0, 2, 3
};
static const size_t test_distribution_size = sizeof(test_distribution) / sizeof(test_distribution[0]);

namespace
{

class TestDB: public cryptonote::BaseTestDB
{
public:
  TestDB(size_t bc_height = test_distribution_size): blockchain_height(bc_height) { m_open = true; }
  virtual uint64_t height() const override { return blockchain_height; }

  std::vector<uint64_t> get_block_cumulative_rct_outputs(const std::vector<uint64_t> &heights) const override
  {
    std::vector<uint64_t> d;
    for (uint64_t h: heights)
    {
      uint64_t c = 0;
      for (uint64_t i = 0; i <= h; ++i)
        c += test_distribution[i];
      d.push_back(c);
    }
    return d;
  }

  std::vector<uint64_t> get_block_weights(uint64_t start_offset, size_t count) const override
  {
    std::vector<uint64_t> weights;
    while (count--) weights.push_back(1);
    return weights;
  }

  uint64_t blockchain_height;
};

}

bool get_output_distribution(uint64_t amount, uint64_t from, uint64_t to, uint64_t &start_height, std::vector<uint64_t> &distribution, uint64_t &base)
{
  struct get_test_options {
    const std::pair<uint8_t, uint64_t> hard_forks[2];
    const cryptonote::test_options test_options = {
      hard_forks
    };
    get_test_options():hard_forks{std::make_pair((uint8_t)1, (uint64_t)0), std::make_pair((uint8_t)0, (uint64_t)0)}{}
  } opts;
  cryptonote::BlockchainAndPool bap;
  bool r = bap.blockchain.init(new TestDB(test_distribution_size), cryptonote::FAKECHAIN, true, &opts.test_options, 0, NULL);
  return r && bap.blockchain.get_output_distribution(amount, from, to, start_height, distribution, base);
}

crypto::hash get_block_hash(uint64_t height)
{
  crypto::hash hash = crypto::null_hash;
  *((uint64_t*)&hash) = height;
  return hash;
}

TEST(output_distribution, extend)
{
  boost::optional<cryptonote::rpc::output_distribution_data> res;

  res = cryptonote::rpc::RpcHandler::get_output_distribution(::get_output_distribution, 0, 28, 29, ::get_block_hash, false, test_distribution_size);
  ASSERT_TRUE(res != boost::none);
  ASSERT_EQ(res->distribution.size(), 2);
  ASSERT_EQ(res->distribution, std::vector<uint64_t>({5, 0}));

  res = cryptonote::rpc::RpcHandler::get_output_distribution(::get_output_distribution, 0, 28, 29, ::get_block_hash, true, test_distribution_size);
  ASSERT_TRUE(res != boost::none);
  ASSERT_EQ(res->distribution.size(), 2);
  ASSERT_EQ(res->distribution, std::vector<uint64_t>({55, 55}));

  res = cryptonote::rpc::RpcHandler::get_output_distribution(::get_output_distribution, 0, 28, 30, ::get_block_hash, false, test_distribution_size);
  ASSERT_TRUE(res != boost::none);
  ASSERT_EQ(res->distribution.size(), 3);
  ASSERT_EQ(res->distribution, std::vector<uint64_t>({5, 0, 2}));

  res = cryptonote::rpc::RpcHandler::get_output_distribution(::get_output_distribution, 0, 28, 30, ::get_block_hash, true, test_distribution_size);
  ASSERT_TRUE(res != boost::none);
  ASSERT_EQ(res->distribution.size(), 3);
  ASSERT_EQ(res->distribution, std::vector<uint64_t>({55, 55, 57}));

  res = cryptonote::rpc::RpcHandler::get_output_distribution(::get_output_distribution, 0, 28, 31, ::get_block_hash, false, test_distribution_size);
  ASSERT_TRUE(res != boost::none);
  ASSERT_EQ(res->distribution.size(), 4);
  ASSERT_EQ(res->distribution, std::vector<uint64_t>({5, 0, 2, 3}));

  res = cryptonote::rpc::RpcHandler::get_output_distribution(::get_output_distribution, 0, 28, 31, ::get_block_hash, true, test_distribution_size);
  ASSERT_TRUE(res != boost::none);
  ASSERT_EQ(res->distribution.size(), 4);
  ASSERT_EQ(res->distribution, std::vector<uint64_t>({55, 55, 57, 60}));
}

TEST(output_distribution, one)
{
  boost::optional<cryptonote::rpc::output_distribution_data> res;

  res = cryptonote::rpc::RpcHandler::get_output_distribution(::get_output_distribution, 0, 0, 0, ::get_block_hash, false, test_distribution_size);
  ASSERT_TRUE(res != boost::none);
  ASSERT_EQ(res->distribution.size(), 1);
  ASSERT_EQ(res->distribution.back(), 0);
}

TEST(output_distribution, full_cumulative)
{
  boost::optional<cryptonote::rpc::output_distribution_data> res;

  res = cryptonote::rpc::RpcHandler::get_output_distribution(::get_output_distribution, 0, 0, 31, ::get_block_hash, true, test_distribution_size);
  ASSERT_TRUE(res != boost::none);
  ASSERT_EQ(res->distribution.size(), 32);
  ASSERT_EQ(res->distribution.back(), 60);
}

TEST(output_distribution, full_noncumulative)
{
  boost::optional<cryptonote::rpc::output_distribution_data> res;

  res = cryptonote::rpc::RpcHandler::get_output_distribution(::get_output_distribution, 0, 0, 31, ::get_block_hash, false, test_distribution_size);
  ASSERT_TRUE(res != boost::none);
  ASSERT_EQ(res->distribution.size(), 32);
  for (size_t i = 0; i < 32; ++i)
    ASSERT_EQ(res->distribution[i], test_distribution[i]);
}

TEST(output_distribution, part_cumulative)
{
  boost::optional<cryptonote::rpc::output_distribution_data> res;

  res = cryptonote::rpc::RpcHandler::get_output_distribution(::get_output_distribution, 0, 4, 8, ::get_block_hash, true, test_distribution_size);
  ASSERT_TRUE(res != boost::none);
  ASSERT_EQ(res->distribution.size(), 5);
  ASSERT_EQ(res->distribution, std::vector<uint64_t>({0, 1, 6, 7, 11}));
}

TEST(output_distribution, part_noncumulative)
{
  boost::optional<cryptonote::rpc::output_distribution_data> res;

  res = cryptonote::rpc::RpcHandler::get_output_distribution(::get_output_distribution, 0, 4, 8, ::get_block_hash, false, test_distribution_size);
  ASSERT_TRUE(res != boost::none);
  ASSERT_EQ(res->distribution.size(), 5);
  ASSERT_EQ(res->distribution, std::vector<uint64_t>({0, 1, 5, 1, 4}));
}
