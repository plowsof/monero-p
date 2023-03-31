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
#include <iostream>
#include <stdexcept>
#include <unordered_map>

#include "one_span_compression.h"
#include "varint.h"

static constexpr unsigned char ONE_SPAN_BIT = 0x80;

namespace tools
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
} // namespace tools
