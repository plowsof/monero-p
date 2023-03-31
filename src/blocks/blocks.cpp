#include "blocks.h"
#include "common/one_span_compression.h"
#include "common/varint.h"

#include <iostream>
#include <unordered_map>

extern const unsigned char checkpoints[];
extern const size_t checkpoints_len;
extern const unsigned char stagenet_blocks[];
extern const size_t stagenet_blocks_len;
extern const unsigned char testnet_blocks[];
extern const size_t testnet_blocks_len;

extern const unsigned char mainnet_cb_dist[];
extern const size_t mainnet_cb_dist_len;
extern const unsigned char testnet_cb_dist[];
extern const size_t testnet_cb_dist_len;

namespace blocks
{
  const std::unordered_map<cryptonote::network_type, const epee::span<const unsigned char>, std::hash<size_t>> CheckpointsByNetwork = {
    {cryptonote::network_type::MAINNET, {checkpoints, checkpoints_len}},
    {cryptonote::network_type::STAGENET, {stagenet_blocks, stagenet_blocks_len}},
    {cryptonote::network_type::TESTNET, {testnet_blocks, testnet_blocks_len}}
  };

  const epee::span<const unsigned char> GetCheckpointsData(cryptonote::network_type network)
  {
    const auto it = CheckpointsByNetwork.find(network);
    if (it != CheckpointsByNetwork.end())
    {
      return it->second;
    }
    return nullptr;
  }

  const epee::span<const uint64_t> GetRCTCoinbaseOutputDist(cryptonote::network_type network)
  {
    static const std::vector<uint64_t> mainnet_data = tools::decompress_one_span_format(std::string(
      reinterpret_cast<const char*>(mainnet_cb_dist), mainnet_cb_dist_len));
    static const std::vector<uint64_t> stagenet_data(1252000, 1);
    static const std::vector<uint64_t> testnet_data = tools::decompress_one_span_format(std::string(
      reinterpret_cast<const char*>(testnet_cb_dist), testnet_cb_dist_len));
    static const std::vector<uint64_t> fakechain_data{};

    switch (network)
    {
      case cryptonote::STAGENET: return epee::to_span(stagenet_data);
      case cryptonote::TESTNET: return epee::to_span(testnet_data);
      case cryptonote::MAINNET: return epee::to_span(mainnet_data);
      case cryptonote::FAKECHAIN: return epee::to_span(fakechain_data);
      default: throw std::logic_error("unrecognized network type");
    }
  }
}
