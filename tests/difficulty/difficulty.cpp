// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 
// 
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>

#include "misc_log_ex.h"
#include "cryptonote_config.h"
#include "cryptonote_basic/difficulty.h"

using namespace std;

#define DEFAULT_TEST_DIFFICULTY_TARGET        120

static int test_wide_difficulty(const char *filename)
{
    std::vector<uint64_t> timestamps;
    std::vector<cryptonote::difficulty_type> cumulative_difficulties;
    fstream data(filename, fstream::in);
    data.exceptions(fstream::badbit);
    data.clear(data.rdstate());
    uint64_t timestamp;
    cryptonote::difficulty_type difficulty, cumulative_difficulty = 0;
    size_t n = 0;
    while (data >> timestamp >> difficulty) {
        size_t begin, end;
        if (n < DIFFICULTY_WINDOW + DIFFICULTY_LAG) {
            begin = 0;
            end = min(n, (size_t) DIFFICULTY_WINDOW);
        } else {
            end = n - DIFFICULTY_LAG;
            begin = end - DIFFICULTY_WINDOW;
        }
        cryptonote::difficulty_type res = cryptonote::next_difficulty(
            std::vector<uint64_t>(timestamps.begin() + begin, timestamps.begin() + end),
            std::vector<cryptonote::difficulty_type>(cumulative_difficulties.begin() + begin, cumulative_difficulties.begin() + end), DEFAULT_TEST_DIFFICULTY_TARGET);
        if (res != difficulty) {
            cerr << "Wrong wide difficulty for block " << n << endl
                << "Expected: " << difficulty << endl
                << "Found: " << res << endl;
            return 1;
        }
        timestamps.push_back(timestamp);
        cumulative_difficulties.push_back(cumulative_difficulty += difficulty);
        ++n;
    }
    if (!data.eof()) {
        data.clear(fstream::badbit);
    }
    return 0;
}

int main(int argc, char *argv[]) {
    TRY_ENTRY();

    if (argc < 2) {
        cerr << "Wrong arguments" << endl;
        return 1;
    }
    if (argc == 3 && strcmp(argv[1], "--wide") == 0)
    {
        return test_wide_difficulty(argv[2]);
    }

    vector<uint64_t> timestamps, cumulative_difficulties;
    std::vector<cryptonote::difficulty_type> wide_cumulative_difficulties;
    fstream data(argv[1], fstream::in);
    data.exceptions(fstream::badbit);
    data.clear(data.rdstate());
    uint64_t timestamp;
    uint64_t difficulty, cumulative_difficulty = 0;
    cryptonote::difficulty_type wide_cumulative_difficulty = 0;
    size_t n = 0;
    while (data >> timestamp >> difficulty) {
        size_t begin, end;
        if (n < DIFFICULTY_WINDOW + DIFFICULTY_LAG) {
            begin = 0;
            end = min(n, (size_t) DIFFICULTY_WINDOW);
        } else {
            end = n - DIFFICULTY_LAG;
            begin = end - DIFFICULTY_WINDOW;
        }
        uint64_t res = cryptonote::next_difficulty_64(
            vector<uint64_t>(timestamps.begin() + begin, timestamps.begin() + end),
            std::vector<uint64_t>(cumulative_difficulties.begin() + begin, cumulative_difficulties.begin() + end), DEFAULT_TEST_DIFFICULTY_TARGET);
        if (res != difficulty) {
            cerr << "Wrong difficulty for block " << n << endl
                << "Expected: " << difficulty << endl
                << "Found: " << res << endl;
            return 1;
        }
        cryptonote::difficulty_type wide_res = cryptonote::next_difficulty(
            std::vector<uint64_t>(timestamps.begin() + begin, timestamps.begin() + end),
            std::vector<cryptonote::difficulty_type>(wide_cumulative_difficulties.begin() + begin, wide_cumulative_difficulties.begin() + end), DEFAULT_TEST_DIFFICULTY_TARGET);
        if ((wide_res & 0xffffffffffffffff).convert_to<uint64_t>() != res) {
            cerr << "Wrong wide difficulty for block " << n << endl
                << "Expected: " << res << endl
                << "Found: " << wide_res << endl;
            return 1;
        }
        timestamps.push_back(timestamp);
        cumulative_difficulties.push_back(cumulative_difficulty += difficulty);
        wide_cumulative_difficulties.push_back(wide_cumulative_difficulty += difficulty);
        ++n;
    }
    if (!data.eof()) {
        data.clear(fstream::badbit);
    }
    return 0;

    CATCH_ENTRY_L0("main", 1);
}
