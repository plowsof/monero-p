// Copyright (c) 2023, The Monero Project
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this list of
//    conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice, this list
//    of conditions and the following disclaimer in the documentation and/or other
//    materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors may be
//    used to endorse or promote products derived from this software without specific
//    prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
// THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <algorithm>
#include <stdexcept>
#include <unordered_map>

#include "one_span_compression.h"
#include "varint.h"

static constexpr unsigned char ONE_SPAN_BIT = 0x80;
static constexpr unsigned char UNCOMMON_VALUE_MARKER = 0x7f;

namespace
{
void histogram_increment(std::unordered_map<uint64_t, size_t>& histogram, uint64_t value)
{
    const auto hit = histogram.find(value);
    if (hit != histogram.end())
        hit->second++;
    else
        histogram.insert({value, 1});
}
} // anonymous namespace

namespace tools
{
std::string compress_one_span_format(const std::vector<uint64_t>& data)
{
    std::vector<std::pair<bool, uint64_t>> span_variants;
    std::unordered_map<uint64_t, size_t> value_histogram;

    std::string s;
    // table size byte + (max table elements + max varint size) +
    // (uncommon value marker + max varint size) * numvals
    s.resize(1 + 127 * 8 + 9 * data.size());

    // Construct span_variants and value histogram
    // For each element in span variants if, the bool is true, decompression should insert x number
    // of 1 elements. If the value is false, decompression should insert the value x itself
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
                histogram_increment(value_histogram, current_one_span);
                current_one_span = 0;
            }
            span_variants.push_back({false, v});
            histogram_increment(value_histogram, v);
        }
    }
    if (current_one_span)
    {
        span_variants.push_back({true, current_one_span});
        histogram_increment(value_histogram, current_one_span);
    }

    // Sort the histogram values by most values most commonly occurring
    using hist_val_t = std::pair<uint64_t, size_t>;
    std::vector<hist_val_t> sorted_histogram(value_histogram.cbegin(), value_histogram.cend());
    std::sort(sorted_histogram.begin(), sorted_histogram.end(),
        [](const hist_val_t& l, const hist_val_t& r) -> bool { return l.second > r.second; });

    // Calculate lookup table size and write it to the output
    const uint8_t lookup_table_size = static_cast<uint8_t>(
        std::min(sorted_histogram.size(), (size_t) 127));
    uint8_t* p = reinterpret_cast<uint8_t*>(&s[0]);
    *(p++) = lookup_table_size;

    // Construct lookup table itself while writing it to the output as packed varint list
    // The lookup table contains the 127 most common values in the span variants data
    std::unordered_map<uint64_t, unsigned char> lookup_index_by_value;
    for (uint8_t i = 0; i < lookup_table_size; ++i)
    {
        const uint64_t& val = sorted_histogram[i].first;
        lookup_index_by_value.insert({val, i});
        tools::write_varint(p, val);
    }

    // Write number of data elements as a varint
    tools::write_varint(p, data.size());

    // Write values in span_variants as its index in the lookup table & ONE_SPAN_BIT if a 1-span OR
    // write the ONE_SPAN_BIT (if 1-span) ored with UNCOMMON_VALUE_MARKER then the value as a varint
    for (const auto& pair : span_variants)
    {
        const bool is_one_span = pair.first;
        const uint64_t& v = pair.second;
        const auto tabit = lookup_index_by_value.find(v);
        uint8_t byte_head = is_one_span ? ONE_SPAN_BIT : 0;
        if (tabit != lookup_index_by_value.end()) // common value
        {
            byte_head |= tabit->second;
            *(p++) = byte_head;
        }
        else // uncommon value
        {
            byte_head |= UNCOMMON_VALUE_MARKER;
            *(p++) = byte_head;
            tools::write_varint(p, v);
        }
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
} // namespace tools
