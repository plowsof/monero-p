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

#include "misc_log_ex.h"
#include "one_span_compression.h"
#include "varint.h"

#undef MONERO_DEFAULT_LOG_CATEGORY
#define MONERO_DEFAULT_LOG_CATEGORY "onespan"

static constexpr unsigned char ONE_SPAN_BIT = 0x80;
static constexpr unsigned char TABLE_INDEX_BIT = 0x40;
static constexpr unsigned char UNCOMMON_VALUE_MARKER = 0x3f;
static constexpr size_t MAX_DECOMPRESS_SIZE = 1 << 23; // ~8 million

namespace
{
void histogram_increment_if_big(std::unordered_map<uint64_t, size_t>& histogram, uint64_t value)
{
    if (value <= 63)
    {
        return;
    }
    const auto hit = histogram.find(value);
    if (hit != histogram.end())
    {
        hit->second++;
    }
    else
    {
        histogram.insert({value, 1});
    }
}

size_t value_stream_size(uint64_t value)
{
    if (0 == value) return 1;
    size_t log128 = 0;
    while (value)
    {
        value >>= 7;
        ++log128;
    }
    return log128;
}
} // anonymous namespace

namespace tools
{
std::string compress_one_span_format(const std::vector<uint64_t>& data)
{
    if (data.size() > MAX_DECOMPRESS_SIZE)
    {
        throw std::logic_error("Data has too many elements");
    }

    std::vector<std::pair<bool, uint64_t>> span_variants;
    std::unordered_map<uint64_t, size_t> big_value_histogram;

    std::string s;
    // table size byte + (max table elements + max varint size) + num element varint
    // (uncommon value marker + max varint size) * numvals
    s.resize(1 + 63 * 8 + 8 + (10 + 1) * data.size());

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
                histogram_increment_if_big(big_value_histogram, current_one_span);
                current_one_span = 0;
            }
            span_variants.push_back({false, v});
            histogram_increment_if_big(big_value_histogram, v);
        }
    }
    if (current_one_span)
    {
        span_variants.push_back({true, current_one_span});
        histogram_increment_if_big(big_value_histogram, current_one_span);
    }

    // Sort the histogram values by most values most commonly occurring
    using hist_val_t = std::pair<uint64_t, size_t>;
    std::vector<hist_val_t> sorted_histogram(big_value_histogram.cbegin(),
        big_value_histogram.cend());
    std::sort
    (
        sorted_histogram.begin(), sorted_histogram.end(),
        [](const hist_val_t& l, const hist_val_t& r) -> bool
        {
            // Compare the greater value of representation size in stream * number of occurrences
            return value_stream_size(l.first) * l.second > value_stream_size(r.first) * r.second;
        }
    );

    // Calculate lookup table size and write it to the output, since 6 bits are available and since
    // we need one value to designate "not in table", the max table size is 63
    const uint8_t lookup_table_size = static_cast<uint8_t>(
        std::min(sorted_histogram.size(), (size_t) 63));
    uint8_t* p = reinterpret_cast<uint8_t*>(&s[0]);
    *(p++) = lookup_table_size;

    // Construct lookup table itself while writing it to the output as packed varint list
    // The lookup table contains the 127 most common values in the span variants data
    std::unordered_map<uint64_t, unsigned char> lookup_index_by_value;
    for (uint8_t i = 0; i < lookup_table_size; ++i)
    {
        const uint64_t& tab_val = sorted_histogram[i].first;
        lookup_index_by_value.insert({tab_val, i});
        tools::write_varint(p, tab_val);
    }

    // Write number of data elements as a varint
    tools::write_varint(p, data.size());

    // Write values in span_variants as its index in the lookup table & ONE_SPAN_BIT if a 1-span OR
    // write the ONE_SPAN_BIT (if 1-span) ored with UNCOMMON_VALUE_MARKER then the value as a varint
    for (const auto& pair : span_variants)
    {
        const bool is_one_span = pair.first;
        const uint64_t& v = pair.second;
        uint8_t byte_head = is_one_span ? ONE_SPAN_BIT : 0;
        const auto tabit = lookup_index_by_value.find(v);
        if (v < UNCOMMON_VALUE_MARKER) // value < 63, fits in 6 bits and != UNCOMMON_VALUE_MARKER
        {
            byte_head |= static_cast<unsigned char>(v);
            *(p++) = byte_head;
        }
        else if (tabit != lookup_index_by_value.end()) // common value > 63
        {
            byte_head |= TABLE_INDEX_BIT;
            byte_head |= tabit->second;
            *(p++) = byte_head;
        }
        else // uncommon value > 63
        {
            byte_head |= UNCOMMON_VALUE_MARKER;
            *(p++) = byte_head;
            tools::write_varint(p, v);
        }
    }

    const size_t actual_size = p - reinterpret_cast<unsigned char*>(&s[0]);
    if (actual_size > s.size())
    {
        MFATAL("Buffer overflow in compress_one_span_format");
        exit(EXIT_FAILURE);
    }
    s.resize(actual_size);

    return s;
}

std::vector<uint64_t> decompress_one_span_format(const std::string& compressed)
{
    if (compressed.empty())
    {
        throw std::runtime_error("Empty compressed string is invalid");
    }
    else if (compressed.size() > MAX_DECOMPRESS_SIZE)
    {
        throw std::runtime_error("Decompresses to too many elements");
    }

    // Read table size as byte
    const unsigned char table_size = *reinterpret_cast<const unsigned char*>(compressed.data()) &
        UNCOMMON_VALUE_MARKER; // ignore 2 MSB

    // Read table values, construct lookup table, and read the number of data elements
    uint64_t lookup_table[64] = {0}; // 63 + 1 b/c num_elements
    auto sit = compressed.cbegin() + 1;
    for (unsigned char i = 0; i <= table_size; ++i)
    {
        const int nread = tools::read_varint(decltype(sit)(sit), compressed.cend(), lookup_table[i]);
        if (nread < 0 || nread > 256)
        {
            throw std::runtime_error("Error decompressing varint table");
        }
        std::advance(sit, nread);
    }
    const size_t num_elements = lookup_table[table_size];

    if (num_elements > MAX_DECOMPRESS_SIZE)
    {
        throw std::runtime_error("Decompresses to too many elements");
    }

    // Read in data values
    std::vector<uint64_t> data;
    data.reserve(num_elements);
    while (sit < compressed.cend())
    {
        unsigned char byte_head = *reinterpret_cast<const unsigned char*>(&*(sit++));
        const bool one_span = byte_head & ONE_SPAN_BIT;
        byte_head &= ~ONE_SPAN_BIT;
        const bool is_table_index = byte_head & TABLE_INDEX_BIT;
        byte_head &= ~TABLE_INDEX_BIT;

        uint64_t encod_val;
        if (is_table_index) // common value > 63
        {
            encod_val = lookup_table[byte_head];
        }
        else if (byte_head != UNCOMMON_VALUE_MARKER) // value < 63
        {
            encod_val = byte_head;
        }
        else // uncommon value > 63
        {
            const int nread = tools::read_varint(decltype(sit)(sit), compressed.cend(), encod_val);
            if (nread < 0 || nread > 256)
            {
                throw std::runtime_error("Error reading uncommon varint data element");
            }
            std::advance(sit, nread);
        }

        if (one_span)
        {
            if (encod_val > MAX_DECOMPRESS_SIZE || MAX_DECOMPRESS_SIZE - encod_val < data.size())
            {
                throw std::runtime_error("Decompresses to too many elements");
            }
            for (uint64_t i = 0; i < encod_val; ++i)
            {
                data.push_back(1);
            }
        }
        else
        {
            data.push_back(encod_val);
        }

        if (data.size() > MAX_DECOMPRESS_SIZE)
        {
            throw std::runtime_error("Decompresses to too many elements");
        }
    }

    if (data.size() != num_elements)
    {
        throw std::runtime_error("Unexpected size at end of decompressesion");
    }

    return data;
}
} // namespace tools
