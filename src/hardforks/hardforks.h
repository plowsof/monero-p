// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
//

#pragma once

#include <stdint.h>
#include <time.h>

struct hardfork_t
{
  uint8_t version;
  uint64_t height;
  uint8_t threshold;
  time_t time;
  hardfork_t(uint8_t version, uint64_t height, uint8_t threshold, time_t time): version(version), height(height), threshold(threshold), time(time) {}
};

extern const hardfork_t mainnet_hard_forks[];
extern const uint64_t mainnet_hard_fork_version_1_till;
extern const size_t num_mainnet_hard_forks;

extern const hardfork_t testnet_hard_forks[];
extern const uint64_t testnet_hard_fork_version_1_till;
extern const size_t num_testnet_hard_forks;

extern const hardfork_t stagenet_hard_forks[];
extern const size_t num_stagenet_hard_forks;
