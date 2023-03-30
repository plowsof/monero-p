
#include <algorithm>
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>

#include "cryptonote_core/cryptonote_core.h"

namespace cryptonote
{
namespace rpc
{
  namespace
  {
    output_distribution_data
      process_distribution(bool cumulative, std::uint64_t start_height, std::vector<std::uint64_t> distribution, std::uint64_t base)
    {
      if (!cumulative && !distribution.empty())
      {
        for (std::size_t n = distribution.size() - 1; 0 < n; --n)
          distribution[n] -= distribution[n - 1];
        distribution[0] -= base;
      }

      return {std::move(distribution), start_height, base};
    }
  }

  boost::optional<output_distribution_data>
    RpcHandler::get_output_distribution(const std::function<bool(uint64_t, uint64_t, uint64_t, uint64_t&, std::vector<uint64_t>&, uint64_t&)> &f, uint64_t amount, uint64_t from_height, uint64_t to_height, const std::function<crypto::hash(uint64_t)> &get_hash, bool cumulative, uint64_t blockchain_height)
  {
      static struct D
      {
        boost::mutex mutex;
        std::vector<std::uint64_t> cached_distribution;
        std::uint64_t cached_from, cached_to, cached_start_height, cached_base;
        crypto::hash cached_m10_hash;
        crypto::hash cached_top_hash;
        bool cached;
        D(): cached_from(0), cached_to(0), cached_start_height(0), cached_base(0), cached_m10_hash(crypto::null_hash), cached_top_hash(crypto::null_hash), cached(false) {}
      } d;
      const boost::unique_lock<boost::mutex> lock(d.mutex);

      crypto::hash top_hash = crypto::null_hash;
      if (d.cached_to < blockchain_height)
        top_hash = get_hash(d.cached_to);
      if (d.cached && amount == 0 && d.cached_from == from_height && d.cached_to == to_height && d.cached_top_hash == top_hash)
        return process_distribution(cumulative, d.cached_start_height, d.cached_distribution, d.cached_base);

      std::vector<std::uint64_t> distribution;
      std::uint64_t start_height, base;

      // see if we can extend the cache - a common case
      bool can_extend = d.cached && amount == 0 && d.cached_from == from_height && to_height > d.cached_to && top_hash == d.cached_top_hash;
      if (!can_extend)
      {
        // we kept track of the hash 10 blocks below, if it exists, so if it matches,
        // we can still pop the last 10 cached slots and try again
        if (d.cached && amount == 0 && d.cached_from == from_height && d.cached_to - d.cached_from >= 10 && to_height > d.cached_to - 10)
        {
          crypto::hash hash10 = get_hash(d.cached_to - 10);
          if (hash10 == d.cached_m10_hash)
          {
            d.cached_to -= 10;
            d.cached_top_hash = hash10;
            d.cached_m10_hash = crypto::null_hash;
            CHECK_AND_ASSERT_MES(d.cached_distribution.size() >= 10, boost::none, "Cached distribution size does not match cached bounds");
            for (int p = 0; p < 10; ++p)
              d.cached_distribution.pop_back();
            can_extend = true;
          }
        }
      }
      if (can_extend)
      {
        std::vector<std::uint64_t> new_distribution;
        if (!f(amount, d.cached_to + 1, to_height, start_height, new_distribution, base))
          return boost::none;
        distribution = d.cached_distribution;
        distribution.reserve(distribution.size() + new_distribution.size());
        for (const auto &e: new_distribution)
          distribution.push_back(e);
        start_height = d.cached_start_height;
        base = d.cached_base;
      }
      else
      {
        if (!f(amount, from_height, to_height, start_height, distribution, base))
          return boost::none;
      }

      if (to_height > 0 && to_height >= from_height)
      {
        const std::uint64_t offset = std::max(from_height, start_height);
        if (offset <= to_height && to_height - offset + 1 < distribution.size())
          distribution.resize(to_height - offset + 1);
      }

      if (amount == 0)
      {
        d.cached_from = from_height;
        d.cached_to = to_height;
        d.cached_top_hash = get_hash(d.cached_to);
        d.cached_m10_hash = d.cached_to >= 10 ? get_hash(d.cached_to - 10) : crypto::null_hash;
        d.cached_distribution = distribution;
        d.cached_start_height = start_height;
        d.cached_base = base;
        d.cached = true;
      }

      return process_distribution(cumulative, start_height, std::move(distribution), base);
  }

  bool CoinbaseOutputDistributionCache::get_coinbase_output_distribution
  (
    const uint64_t start_height, // inclusive
    const uint64_t stop_height, // inclusive
    std::vector<uint64_t>& num_cb_outs_per_block,
    bool* only_used_cache/* = nullptr*/
  )
  {
    if (only_used_cache)
      *only_used_cache = true;

    if (stop_height < start_height)
    {
      num_cb_outs_per_block.clear();
      return true;
    }
    else if (stop_height > CRYPTONOTE_MAX_BLOCK_NUMBER)
    {
      MERROR("Request for ridiculous height: " << stop_height);
      return false;
    }

    // Start accessing the cache

    std::lock_guard<decltype(m_mutex)> ll(m_mutex);

    rollback_for_reorgs();

    if (m_num_cb_outs_per_block.empty())
    {
      // If we have nothing cached, we can adjust the cache start height to the first request.
      // Since realistically, we will only use this cache for post-RCT coinbase blocks, we can save
      // some space and DB fetching.
      m_height_begin = start_height;
    }

    const uint64_t num_missing_front = (start_height < m_height_begin) ? (m_height_begin - start_height) : 0;
    const uint64_t num_missing_back = (stop_height >= height_end()) ? (stop_height - height_end() + 1) : 0;

    if (only_used_cache)
      *only_used_cache = num_missing_front || num_missing_back;

    if (!fetch_and_extend(start_height, num_missing_front, true))
    {
      return false;
    }
    else if (!fetch_and_extend(height_end(), num_missing_back, false))
    {
      return false;
    }

    CHECK_AND_ASSERT_MES(m_height_begin <= start_height, false, "internal bug: extend front");
    CHECK_AND_ASSERT_MES(height_end() > stop_height, false, "internal bug: extend back");

    const size_t num_queried_blocks = stop_height - start_height + 1; // +1 since inclusive range
    const auto res_begin = m_num_cb_outs_per_block.begin() + (start_height - m_height_begin);
    const auto res_end = res_begin + num_queried_blocks;
    num_cb_outs_per_block = {res_begin, res_end};

    return true;
  }

  void CoinbaseOutputDistributionCache::rollback_for_reorgs()
  {
    bool top_9_potent_invalid = false;
    bool top_99_potent_invalid = false;
    bool all_potent_invalid = false;

    const uint64_t num_blocks_cached = m_num_cb_outs_per_block.size();
    if (num_blocks_cached >= 1 && m_last_1_hash != m_get_block_id_by_height(height_end() - 1))
    {
      top_9_potent_invalid = true;
      if (num_blocks_cached >= 10 && m_last_10_hash != m_get_block_id_by_height(height_end() - 10))
      {
        top_99_potent_invalid = true;
        if (num_blocks_cached >= 100 && m_last_100_hash != m_get_block_id_by_height(height_end() - 100))
        {
          all_potent_invalid = true;
        }
      }
    }

    if (all_potent_invalid)
    {
      m_num_cb_outs_per_block.clear();
    }
    else if (top_99_potent_invalid)
    {
      for (size_t i = 0; i < 99; ++i) m_num_cb_outs_per_block.pop_back();
    }
    else if (top_9_potent_invalid)
    {
      for (size_t i = 0; i < 9; ++i) m_num_cb_outs_per_block.pop_back();
    }
    else
    {
      return; // Everything is valid! Nothing to do
    }

    save_current_checkpoints();
  }

  bool CoinbaseOutputDistributionCache::fetch_and_extend
  (
    const uint64_t block_start_offset,
    const size_t count,
    const bool cache_front
  )
  {
    CHECK_AND_ASSERT_MES(!(cache_front && count > m_height_begin), false, "Trying to extend front past 0");

    std::vector<uint64_t> extension;
    extension.reserve(count);

    // To save memory, and not hog blockchain locks, we only request MAX_GET_BLOCKS_CHUNK at a time.
    // However, we don't want to fetch part of the blocks, modify our cache accordingly, run into an
    // issue, then abort in an inconsistent state. So, the variable extension will contain a compact
    // list of only the values that are needed. Then, post-fetch, we add all the values in extension
    // to our cache in one go.
    static constexpr size_t MAX_GET_BLOCKS_CHUNK = 1000;
    for (size_t num_fetched = 0; num_fetched < count; num_fetched += MAX_GET_BLOCKS_CHUNK)
    {
      const size_t remaining = count - num_fetched;
      const size_t chunk_size = std::min(remaining, MAX_GET_BLOCKS_CHUNK);
      const size_t chunk_start = block_start_offset + num_fetched;

      std::vector<block> blocks;
      if (!m_get_blocks(chunk_start, chunk_size, blocks))
      {
        MERROR("Could not get blocks " << block_start_offset << "/+" << count);
        return false;
      }

      for (const auto& b : blocks)
      {
        extension.push_back(b.miner_tx.vout.size());
      }
    }

    CHECK_AND_ASSERT_MES(extension.size() == count, false, "internal bug: chunking wrong size");

    if (cache_front)
    {
      for (auto it = extension.rbegin(); it < extension.rend(); ++it)
      {
        m_num_cb_outs_per_block.push_front(*it);
      }

      // Adding blocks to front of the deque changes our starting height
      m_height_begin -= count;
    }
    else
    {
      for (auto it = extension.begin(); it < extension.end(); ++it)
      {
        m_num_cb_outs_per_block.push_back(*it);
      }
    }

    if (count)
    {
      save_current_checkpoints();
    }

    return true;
  }

  void CoinbaseOutputDistributionCache::save_current_checkpoints()
  {
    const uint64_t num_cached = m_num_cb_outs_per_block.size();
    if (num_cached >= 1) m_last_1_hash = m_get_block_id_by_height(height_end() - 1);
    if (num_cached >= 10) m_last_10_hash = m_get_block_id_by_height(height_end() - 10);
    if (num_cached >= 100) m_last_100_hash = m_get_block_id_by_height(height_end() - 100);
  }
} // rpc
} // cryptonote
