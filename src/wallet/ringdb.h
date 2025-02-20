// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

// 

#pragma once

#include <string>
#include <vector>
#include <lmdb.h>
#include "wipeable_string.h"
#include "crypto/crypto.h"
#include "cryptonote_basic/cryptonote_basic.h"

namespace tools
{
  class ringdb
  {
  public:
    ringdb(std::string filename, const std::string &genesis);
    void close();
    ~ringdb();

    bool add_rings(const crypto::chacha_key &chacha_key, const cryptonote::transaction_prefix &tx);
    bool remove_rings(const crypto::chacha_key &chacha_key, const std::vector<crypto::key_image> &key_images);
    bool remove_rings(const crypto::chacha_key &chacha_key, const cryptonote::transaction_prefix &tx);
    bool get_ring(const crypto::chacha_key &chacha_key, const crypto::key_image &key_image, std::vector<uint64_t> &outs);
    bool get_rings(const crypto::chacha_key &chacha_key, const std::vector<crypto::key_image> &key_images, std::vector<std::vector<uint64_t>> &all_outs);
    bool set_ring(const crypto::chacha_key &chacha_key, const crypto::key_image &key_image, const std::vector<uint64_t> &outs, bool relative);
    bool set_rings(const crypto::chacha_key &chacha_key, const std::vector<std::pair<crypto::key_image, std::vector<uint64_t>>> &rings, bool relative);

    bool blackball(const std::pair<uint64_t, uint64_t> &output);
    bool blackball(const std::vector<std::pair<uint64_t, uint64_t>> &outputs);
    bool unblackball(const std::pair<uint64_t, uint64_t> &output);
    bool blackballed(const std::pair<uint64_t, uint64_t> &output);
    bool clear_blackballs();

  private:
    bool blackball_worker(const std::vector<std::pair<uint64_t, uint64_t>> &outputs, int op);

  private:
    std::string filename;
    MDB_env *env;
    MDB_dbi dbi_rings;
    MDB_dbi dbi_blackballs;
  };
}
