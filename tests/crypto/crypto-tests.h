// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 
// 
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#pragma once

#if defined(__cplusplus)
#include "crypto/crypto.h"

extern "C" {
#endif

void setup_random(void);

#if defined(__cplusplus)
}

bool check_scalar(const crypto::ec_scalar &scalar);
void random_scalar(crypto::ec_scalar &res);
void hash_to_scalar(const void *data, std::size_t length, crypto::ec_scalar &res);
void hash_to_point(const crypto::hash &h, crypto::ec_point &res);
void hash_to_ec(const crypto::public_key &key, crypto::ec_point &res);
bool check_ge_p3_identity_failure(const crypto::public_key &point);
bool check_ge_p3_identity_success(const crypto::public_key &point);
#endif
