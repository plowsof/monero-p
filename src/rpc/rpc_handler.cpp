
#include <algorithm>
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>

#include "blocks/blocks.h"
#include "cryptonote_core/cryptonote_core.h"

#define HF_VERSION_RCT HF_VERSION_DYNAMIC_FEE

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
    uint64_t& true_start_height,
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

    true_start_height = std::max(start_height, m_height_begin);

    rollback_for_reorgs();

    const uint64_t nmissing = (stop_height >= height_end()) ? (stop_height - height_end() + 1) : 0;

    if (only_used_cache)
      *only_used_cache = nmissing;

    if (!fetch_and_extend(nmissing))
    {
      return false;
    }

    CHECK_AND_ASSERT_MES(height_end() > stop_height, false, "internal bug: extend back");

    const size_t num_queried_blocks = stop_height - true_start_height + 1; // +1 since inclusive []
    const auto res_begin = m_num_cb_outs_per_block.data() + (true_start_height - m_height_begin); 
    const auto res_end = res_begin + num_queried_blocks;
    num_cb_outs_per_block = std::vector<uint64_t>(res_begin, res_end);

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

    all_potent_invalid = all_potent_invalid
      || (top_9_potent_invalid && num_blocks_cached <= 9)
      || (top_99_potent_invalid && num_blocks_cached <= 99);

    if (all_potent_invalid)
    {
      revert_to_hardcoded_rct_state();
    }
    else if (top_99_potent_invalid)
    {
      for (size_t i = 0; i < 99; ++i) m_num_cb_outs_per_block.pop_back();
      save_current_checkpoints();
    }
    else if (top_9_potent_invalid)
    {
      for (size_t i = 0; i < 9; ++i) m_num_cb_outs_per_block.pop_back();
      save_current_checkpoints();
    }
    else
    {
      return; // Everything is valid! Nothing to do
    }
  }

  bool CoinbaseOutputDistributionCache::fetch_and_extend(const size_t count)
  {
    // To save memory, and not hog blockchain locks, we only request MAX_GET_BLOCKS_CHUNK at a time.
    static constexpr size_t MAX_GET_BLOCKS_CHUNK = 1000;
    const uint64_t pre_extend_height_end = height_end();
    for (size_t num_fetched = 0; num_fetched < count; num_fetched += MAX_GET_BLOCKS_CHUNK)
    {
      const size_t remaining = count - num_fetched;
      const size_t chunk_size = std::min(remaining, MAX_GET_BLOCKS_CHUNK);
      const size_t chunk_start = pre_extend_height_end + num_fetched;

      std::vector<block> blocks;
      if (!m_get_blocks(chunk_start, chunk_size, blocks))
      {
        MERROR("Could not get blocks " << chunk_start << "/+" << chunk_size);
        if (num_fetched)
        {
          // If we have have modifications to the cache that succeeded previously, but fail fetching
          // now, we can still use that previously fetched information
          save_current_checkpoints();
        }
        return false;
      }

      for (const auto& b : blocks)
      {
        if (b.major_version >= HF_VERSION_RCT)
        {
          m_num_cb_outs_per_block.push_back(b.miner_tx.vout.size());
        }
        else
        {
          // On fakechain, the genesis block isn't a RCT block so its miner tx should not be counted
          // towards the coinbase enote count, but everywhere else, this condition should never
          // occur, which is the point of revert_to_hardcoded_rct_state(). This is to match the
          // behavior of get_output_distribution for amount==0 on the fakechain: the start height is
          // 0, but the first element of the returned distribution is 0.
          CHECK_AND_ASSERT_THROW_MES(m_net_type == FAKECHAIN, "Processed non-RCT miner tx off fakechain");
          m_num_cb_outs_per_block.push_back(0);
        }
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

  void CoinbaseOutputDistributionCache::revert_to_hardcoded_rct_state()
  {
    // Set m_height_begin to the height of the first block that RCT was enabled
    switch (m_net_type)
    {
      case STAGENET: m_height_begin = stagenet_hard_forks[3].height; break;
      case TESTNET: m_height_begin = testnet_hard_forks[3].height; break;
      case MAINNET: m_height_begin = mainnet_hard_forks[3].height; break;
      case FAKECHAIN: m_height_begin = 0; break;
      default: throw std::logic_error("unrecognized network type");
    }

    const auto hardcoded_dist = blocks::GetRCTCoinbaseOutputDist(m_net_type);
    if (hardcoded_dist.size())
    {
      m_num_cb_outs_per_block = std::vector<uint64_t>{hardcoded_dist.cbegin(), hardcoded_dist.cend()};
    }
    else
    {
      m_num_cb_outs_per_block.clear();
    }

    save_current_checkpoints();

    // @TODO: check against expected checkpoints

  }
} // rpc
} // cryptonote
