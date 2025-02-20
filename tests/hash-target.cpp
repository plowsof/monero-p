// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 
// 
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <limits>
#include "misc_log_ex.h"
#include "crypto/hash.h"
#include "cryptonote_basic/difficulty.h"

using namespace std;
using cryptonote::check_hash;

int main(int argc, char *argv[]) {
  TRY_ENTRY();
  crypto::hash h;
  for (cryptonote::difficulty_type diff = 1;; diff += 1 + (diff >> 8)) {
    for (uint16_t b = 0; b < 256; b++) {
      memset(&h, b, sizeof(crypto::hash));
      if (check_hash(h, diff) != (b == 0 || diff <= 255 / b)) {
        return 1;
      }
      if (b > 0) {
        memset(&h, 0, sizeof(crypto::hash));
        ((char *) &h)[31] = b;
        if (check_hash(h, diff) != (diff <= 255 / b)) {
          return 2;
        }
      }
    }
    if (diff < numeric_limits<uint64_t>::max() / 256) {
      uint64_t val = 0;
      for (int i = 31; i >= 0; i--) {
        val = val * 256 + 255;
        ((char *) &h)[i] = static_cast<char>(static_cast<uint64_t>(val / diff));
        val %= (diff & 0xffffffffffffffff).convert_to<uint64_t>();
      }
      if (check_hash(h, diff) != true) {
        return 3;
      }
      if (diff > 1) {
        for (int i = 0;; i++) {
          if (i >= 32) {
            abort();
          }
          if (++((char *) &h)[i] != 0) {
            break;
          }
        }
        if (check_hash(h, diff) != false) {
          return 4;
        }
      }
    }
    if (diff + 1 + (diff >> 8) < diff) {
      break;
    }
  }
  return 0;
  CATCH_ENTRY_L0("main", 1);
}
