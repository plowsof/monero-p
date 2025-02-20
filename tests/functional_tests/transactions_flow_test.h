// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 
// 
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

bool transactions_flow_test(std::string& working_folder, 
                            std::string path_source_wallet, 
                            std::string path_target_wallet, 
                            std::string& daemon_addr_a, 
                            std::string& daemon_addr_b, 
                            uint64_t amount_to_transfer, size_t mix_in_factor, size_t transactions_count, size_t transactions_per_second);
