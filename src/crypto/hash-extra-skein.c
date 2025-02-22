// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 
// 
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#include <stddef.h>
#include <stdint.h>

#include "hash-ops.h"
#include "skein.h"

#define SKEIN_HASH_BITLEN HASH_SIZE * 8

void hash_extra_skein(const void *data, size_t length, char *hash) {
  // No need to check for failure b/c skein_hash only fails for invalid hash size
  skein_hash(SKEIN_HASH_BITLEN, data, 8 * length, (uint8_t*)hash);
}
