// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 

#pragma once

#include <string>
#include <boost/thread/mutex.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include "include_base_utils.h"
#include "net/abstract_http_client.h"
#include "rpc/core_rpc_server_commands_defs.h"

namespace tools
{

class NodeRPCProxy
{
public:
  NodeRPCProxy(epee::net_utils::http::abstract_http_client &http_client, boost::recursive_mutex &mutex);

  void invalidate();
  void set_offline(bool offline) { m_offline = offline; }

  boost::optional<std::string> get_rpc_version(uint32_t &rpc_version, std::vector<std::pair<uint8_t, uint64_t>> &daemon_hard_forks, uint64_t &height, uint64_t &target_height);
  boost::optional<std::string> get_height(uint64_t &height);
  void set_height(uint64_t h);
  boost::optional<std::string> get_target_height(uint64_t &height);
  boost::optional<std::string> get_block_weight_limit(uint64_t &block_weight_limit);
  boost::optional<std::string> get_adjusted_time(uint64_t &adjusted_time);
  boost::optional<std::string> get_earliest_height(uint8_t version, uint64_t &earliest_height);
  boost::optional<std::string> get_dynamic_base_fee_estimate(uint64_t grace_blocks, uint64_t &fee);
  boost::optional<std::string> get_dynamic_base_fee_estimate_2021_scaling(uint64_t grace_blocks, std::vector<uint64_t> &fees);
  boost::optional<std::string> get_fee_quantization_mask(uint64_t &fee_quantization_mask);
  boost::optional<std::string> get_transactions(const std::vector<crypto::hash> &txids, const std::function<void(const cryptonote::COMMAND_RPC_GET_TRANSACTIONS::request&, const cryptonote::COMMAND_RPC_GET_TRANSACTIONS::response&, bool)> &f);
  boost::optional<std::string> get_block_header_by_height(uint64_t height, cryptonote::block_header_response &block_header);

private:
  boost::optional<std::string> get_info();

  epee::net_utils::http::abstract_http_client &m_http_client;
  boost::recursive_mutex &m_daemon_rpc_mutex;
  bool m_offline;

  uint64_t m_height;
  uint64_t m_earliest_height[256];
  uint64_t m_dynamic_base_fee_estimate;
  uint64_t m_dynamic_base_fee_estimate_cached_height;
  uint64_t m_dynamic_base_fee_estimate_grace_blocks;
  std::vector<uint64_t> m_dynamic_base_fee_estimate_vector;
  uint64_t m_fee_quantization_mask;
  uint64_t m_adjusted_time;
  uint32_t m_rpc_version;
  uint64_t m_target_height;
  uint64_t m_block_weight_limit;
  time_t m_get_info_time;
  time_t m_height_time;
  time_t m_target_height_time;
  std::vector<std::pair<uint8_t, uint64_t>> m_daemon_hard_forks;
};

}
