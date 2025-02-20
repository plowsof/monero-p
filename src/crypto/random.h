// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 
// 
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#pragma once

#include <stddef.h>

void generate_random_bytes_not_thread_safe(size_t n, void *result);
void add_extra_entropy_not_thread_safe(const void *ptr, size_t bytes);
