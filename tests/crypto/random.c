// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 
// 
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#include "crypto/random.c"

#include "crypto-tests.h"

void setup_random(void) {
    memset(&state, 42, sizeof(union hash_state));
}
