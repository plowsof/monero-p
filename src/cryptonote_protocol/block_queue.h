// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 
// 
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#pragma once

#include <string>
#include <vector>
#include <set>
#include <unordered_set>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/uuid/uuid.hpp>
#include "net/net_utils_base.h"

#undef MONERO_DEFAULT_LOG_CATEGORY
#define MONERO_DEFAULT_LOG_CATEGORY "cn.block_queue"

namespace cryptonote
{
  struct block_complete_entry;

  class block_queue
  {
  public:
    struct span
    {
      uint64_t start_block_height;
      std::vector<crypto::hash> hashes;
      std::vector<cryptonote::block_complete_entry> blocks;
      boost::uuids::uuid connection_id;
      uint64_t nblocks;
      float rate;
      size_t size;
      boost::posix_time::ptime time;
      epee::net_utils::network_address origin{};

      span(uint64_t start_block_height, std::vector<cryptonote::block_complete_entry> blocks, const boost::uuids::uuid &connection_id, const epee::net_utils::network_address &addr, float rate, size_t size):
        start_block_height(start_block_height), blocks(std::move(blocks)), connection_id(connection_id), nblocks(this->blocks.size()), rate(rate), size(size), time(boost::date_time::min_date_time), origin(addr) {}
      span(uint64_t start_block_height, uint64_t nblocks, const boost::uuids::uuid &connection_id, const epee::net_utils::network_address &addr, boost::posix_time::ptime time):
        start_block_height(start_block_height), connection_id(connection_id), nblocks(nblocks), rate(0.0f), size(0), time(time), origin(addr) {}

      bool operator<(const span &s) const { return start_block_height < s.start_block_height; }
    };
    typedef std::set<span> block_map;

  public:
    void add_blocks(uint64_t height, std::vector<cryptonote::block_complete_entry> bcel, const boost::uuids::uuid &connection_id, const epee::net_utils::network_address &addr, float rate, size_t size);
    void add_blocks(uint64_t height, uint64_t nblocks, const boost::uuids::uuid &connection_id, const epee::net_utils::network_address &addr, boost::posix_time::ptime time = boost::date_time::min_date_time);
    void flush_spans(const boost::uuids::uuid &connection_id, bool all = false);
    void flush_stale_spans(const std::set<boost::uuids::uuid> &live_connections);
    bool remove_span(uint64_t start_block_height, std::vector<crypto::hash> *hashes = NULL);
    void remove_spans(const boost::uuids::uuid &connection_id, uint64_t start_block_height);
    uint64_t get_max_block_height() const;
    void print() const;
    std::string get_overview(uint64_t blockchain_height) const;
    bool has_unpruned_height(uint64_t block_height, uint64_t blockchain_height, uint32_t pruning_seed) const;
    std::pair<uint64_t, uint64_t> reserve_span(uint64_t first_block_height, uint64_t last_block_height, uint64_t max_blocks, const boost::uuids::uuid &connection_id, const epee::net_utils::network_address &addr, bool sync_pruned_blocks, uint32_t local_pruning_seed, uint32_t pruning_seed, uint64_t blockchain_height, const std::vector<std::pair<crypto::hash, uint64_t>> &block_hashes, boost::posix_time::ptime time = boost::posix_time::microsec_clock::universal_time());
    uint64_t get_next_needed_height(uint64_t blockchain_height) const;
    std::pair<uint64_t, uint64_t> get_next_span_if_scheduled(std::vector<crypto::hash> &hashes, boost::uuids::uuid &connection_id, boost::posix_time::ptime &time) const;
    void reset_next_span_time(boost::posix_time::ptime t = boost::posix_time::microsec_clock::universal_time());
    void set_span_hashes(uint64_t start_height, const boost::uuids::uuid &connection_id, std::vector<crypto::hash> hashes);
    bool get_next_span(uint64_t &height, std::vector<cryptonote::block_complete_entry> &bcel, boost::uuids::uuid &connection_id, epee::net_utils::network_address &addr, bool filled = true) const;
    bool has_next_span(const boost::uuids::uuid &connection_id, bool &filled, boost::posix_time::ptime &time) const;
    bool has_next_span(uint64_t height, bool &filled, boost::posix_time::ptime &time, boost::uuids::uuid &connection_id) const;
    size_t get_data_size() const;
    size_t get_num_filled_spans_prefix() const;
    size_t get_num_filled_spans() const;
    crypto::hash get_last_known_hash(const boost::uuids::uuid &connection_id) const;
    bool has_spans(const boost::uuids::uuid &connection_id) const;
    float get_speed(const boost::uuids::uuid &connection_id) const;
    float get_download_rate(const boost::uuids::uuid &connection_id) const;
    bool foreach(std::function<bool(const span&)> f) const;
    bool requested(const crypto::hash &hash) const;
    bool have(const crypto::hash &hash) const;
    std::uint64_t have_height(const crypto::hash &hash) const;

  private:
    void erase_block(block_map::iterator j);
    inline bool requested_internal(const crypto::hash &hash) const;

  private:
    block_map blocks;
    mutable boost::recursive_mutex mutex;
    std::unordered_set<crypto::hash> requested_hashes;
    std::unordered_map<crypto::hash, std::uint64_t> have_blocks;
  };
}
