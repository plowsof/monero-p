#include "blocks.h"
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

static constexpr unsigned char ONE_SPAN_BIT = 0x80;

namespace blocks
{
  std::string compress_one_span_format(const std::vector<uint64_t>& data)
  {
    std::vector<std::pair<bool, uint64_t>> span_variants;
    std::unordered_map<uint64_t, unsigned char> lookup_index_by_value;

    std::string s;
    s.resize(1 + 128 * 8 + data.size());

    // Construct span_variants and lookup_index_by_value
    uint64_t current_one_span = 0;
    for (const uint64_t v: data)
    {
      if (v == 1)
      {
        current_one_span += 1;
      }
      else
      {
        if (current_one_span)
        {
          span_variants.push_back({true, current_one_span});
          lookup_index_by_value.insert({current_one_span, lookup_index_by_value.size()});
          current_one_span = 0;
        }
        span_variants.push_back({false, v});
        lookup_index_by_value.insert({v, lookup_index_by_value.size()});
      }
      if (lookup_index_by_value.size() >= 128)
      {
        throw std::logic_error("data is too detailed and cannot be compressed well");
      }
    }
    if (current_one_span)
    {
      span_variants.push_back({true, current_one_span});
      lookup_index_by_value.insert({current_one_span, lookup_index_by_value.size()});
    }
    if (lookup_index_by_value.size() >= 128)
    {
      throw std::logic_error("data is too detailed and cannot be compressed well");
    }

    // Write length of lookup table as byte
    unsigned char* p = reinterpret_cast<unsigned char*>(&s[0]);
    *(p++) = static_cast<unsigned char>(lookup_index_by_value.size());

    // Write values in lookup table in order as packed varints
    for (unsigned char i = 0; i < lookup_index_by_value.size(); ++i)
    {
      const auto tval_it = std::find_if(lookup_index_by_value.cbegin(), lookup_index_by_value.cend(),
        [i](const decltype(lookup_index_by_value)::value_type& vt) { return vt.second == i; });
      if (tval_it == lookup_index_by_value.cend())
      {
        throw std::runtime_error("internal bug: reverse lookup table could not find index");
      }
      tools::write_varint(p, tval_it->first);
    }

    // Write number of data elements as a varint
    tools::write_varint(p, data.size());

    // Write values in span_variants as its index in the lookup table & ONE_SPAN_BIT if a 1-span
    for (const auto& pair : span_variants)
    {
      unsigned char val_index = lookup_index_by_value[pair.second];
      if (pair.first) // if one-span
      {
        val_index |= ONE_SPAN_BIT;
      }
      *(p++) = val_index;
    }

    if (p > reinterpret_cast<unsigned char*>(&s[0] + s.size()))
    {
      std::cerr << "horrible internal bug: buffer overflow in compress_one_span_format" << std::endl;
      exit(EXIT_FAILURE);
    }

    s.resize(p - reinterpret_cast<unsigned char*>(&s[0]));

    return s;
  }

  // @TODO: we don't know how much to reserve
  std::vector<uint64_t> decompress_one_span_format(const std::string& compressed)
  {
    // Read table size as byte
    const unsigned char table_size = *reinterpret_cast<const unsigned char*>(compressed.data());
    std::unordered_map<unsigned char, uint64_t> value_by_table_index;

    // Read table values, construct lookup map, and read the number of data elements
    auto sit = compressed.cbegin() + 1;
    uint64_t num_elements;
    for (unsigned char i = 0; i <= table_size; ++i)
    {
      uint64_t tab_val;
      const int nread = tools::read_varint(decltype(sit)(sit), compressed.cend(), tab_val);
      if (nread < 0 || nread > 256)
      {
        throw std::runtime_error("Error decompressing varint table");
      }
      std::advance(sit, nread);
      if (i == table_size)
      {
        num_elements = tab_val;
      }
      else
      {
        value_by_table_index.insert({i, tab_val});
      }
    }

    // Read in data values
    std::vector<uint64_t> data;
    data.reserve(num_elements);
    for (; sit < compressed.cend(); ++sit)
    {
      unsigned char tab_index = *reinterpret_cast<const unsigned char*>(&*sit);
      const bool one_span = tab_index & ONE_SPAN_BIT;
      tab_index &= ~ONE_SPAN_BIT;
      const auto tab_it = value_by_table_index.find(tab_index);
      if (tab_it == value_by_table_index.end())
      {
        throw std::runtime_error("Bad data lookup values");
      }
      const uint64_t data_value = tab_it->second;
      if (one_span)
      {
        for (size_t i = 0; i < data_value; ++i)
        {
          data.push_back(1);
        }
      }
      else
      {
        data.push_back(data_value);
      }
    }

    return data;
  }

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
    static const std::vector<uint64_t> mainnet_data = decompress_one_span_format(std::string(
      reinterpret_cast<const char*>(mainnet_cb_dist), mainnet_cb_dist_len));
    static const std::vector<uint64_t> stagenet_data(1252000, 1);
    static const std::vector<uint64_t> testnet_data = decompress_one_span_format(std::string(
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
