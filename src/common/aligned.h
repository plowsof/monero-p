// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

// 

#pragma once 

#ifdef __cplusplus
extern "C" {
#endif

void *aligned_malloc(size_t bytes, size_t align);
void *aligned_realloc(void *ptr, size_t bytes, size_t align);
void aligned_free(void *ptr);

#ifdef __cplusplus
}
#endif
