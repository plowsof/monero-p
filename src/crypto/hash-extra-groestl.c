// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 
// 
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#include <stddef.h>
#include <stdint.h>

#include "groestl.h"

void hash_extra_groestl(const void *data, size_t length, char *hash) {
  groestl(data, length * 8, (uint8_t*)hash);
}
