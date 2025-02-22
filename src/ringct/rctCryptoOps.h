// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 
// 
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#pragma once

extern "C" {
#include "crypto/crypto-ops.h"
}

void sc_reduce32copy(unsigned char * scopy, const unsigned char *s);
