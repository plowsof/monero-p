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

#include <iostream>
#include <fstream>

#include "gtest/gtest.h"

#include "common/one_span_compression.h"

static void test_start_finish(const std::vector<uint64_t>& data)
{
    std::vector<uint64_t> decomp_data = tools::decompress_one_span_format(
        tools::compress_one_span_format(data));
    ASSERT_EQ(data, decomp_data);
}

TEST(GetRCTCoinbaseOutputDist, one_span_format_start_finish)
{
    test_start_finish({1, 1, 1, 1, 1, 4, 1, 8, 1, 1, 2, 3});
    test_start_finish({});
    test_start_finish({7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7});
    test_start_finish({1, 1, 1, 1, 1});
    test_start_finish({0});
    test_start_finish({1, 5, 7, 3, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 6, 3, 7, 3, 2});
    test_start_finish({1});
    test_start_finish({std::numeric_limits<uint64_t>::max()});
}

/*
TEST(GetRCTCoinbaseOutputDist, one_span_format_testnet)
{
    std::vector<uint64_t> mainnet_data;
    std::ifstream ifs("tests/unit_tests/testnet2207000_2.number");
    do
    {
        uint64_t v;
        if (ifs >> v)
            mainnet_data.push_back(v);
    } while (!ifs.eof());
    std::cout << "Number of data elements: " << mainnet_data.size() << std::endl;

    const std::string compressed_mainnet_data = compress_one_span_format(mainnet_data);
    std::cout << "compressed length: " << compressed_mainnet_data.size() << std::endl;
    std::ofstream ofs("get_rct_dist_compressed_testnet_2207_2.dat");
    ofs.write(compressed_mainnet_data.data(), compressed_mainnet_data.size());
    ofs.close();

    std::cout << "Occurences of 1337: " << std::count(mainnet_data.begin(), mainnet_data.end(), 1337) << std::endl;


    const std::vector<uint64_t> decompressed = decompress_one_span_format(compressed_mainnet_data);
    EXPECT_EQ(mainnet_data, decompressed);

    std::cout << "value at [176139]: " << mainnet_data[176139] << std::endl;
    std::cout << "value at [176139]: " << decompressed[176139] << std::endl;

    std::cout << "Occurences of 1337: " << std::count(decompressed.begin(), decompressed.end(), 1337) << std::endl;
}
*/
