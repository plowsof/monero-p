// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 
// 
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#ifndef _BLAKE256_H_
#define _BLAKE256_H_

#include <stdint.h>

typedef struct {
  uint32_t h[8], s[4], t[2];
  int buflen, nullt;
  uint8_t buf[64];
} state;

typedef struct {
  state inner;
  state outer;
} hmac_state;

void blake256_init(state *);
void blake224_init(state *);

void blake256_update(state *, const uint8_t *, uint64_t);
void blake224_update(state *, const uint8_t *, uint64_t);

void blake256_final(state *, uint8_t *);
void blake224_final(state *, uint8_t *);

void blake256_hash(uint8_t *, const uint8_t *, uint64_t);
void blake224_hash(uint8_t *, const uint8_t *, uint64_t);

/* HMAC functions: */

void hmac_blake256_init(hmac_state *, const uint8_t *, uint64_t);
void hmac_blake224_init(hmac_state *, const uint8_t *, uint64_t);

void hmac_blake256_update(hmac_state *, const uint8_t *, uint64_t);
void hmac_blake224_update(hmac_state *, const uint8_t *, uint64_t);

void hmac_blake256_final(hmac_state *, uint8_t *);
void hmac_blake224_final(hmac_state *, uint8_t *);

void hmac_blake256_hash(uint8_t *, const uint8_t *, uint64_t, const uint8_t *, uint64_t);
void hmac_blake224_hash(uint8_t *, const uint8_t *, uint64_t, const uint8_t *, uint64_t);

#endif /* _BLAKE256_H_ */
