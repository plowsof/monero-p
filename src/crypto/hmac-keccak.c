// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
//

#include "hmac-keccak.h"
#include "memwipe.h"

#define KECCAK_BLOCKLEN 136
#define HASH_SIZE 32

void hmac_keccak_init(hmac_keccak_state *S, const uint8_t *_key, size_t keylen) {
  const uint8_t *key = _key;
  uint8_t keyhash[HASH_SIZE];
  uint8_t pad[KECCAK_BLOCKLEN];
  uint64_t i;

  if (keylen > KECCAK_BLOCKLEN) {
    keccak(key, keylen, keyhash, HASH_SIZE);
    key = keyhash;
    keylen = HASH_SIZE;
  }

  keccak_init(&S->inner);
  memset(pad, 0x36, KECCAK_BLOCKLEN);
  for (i = 0; i < keylen; ++i) {
    pad[i] ^= key[i];
  }
  keccak_update(&S->inner, pad, KECCAK_BLOCKLEN);

  keccak_init(&S->outer);
  memset(pad, 0x5c, KECCAK_BLOCKLEN);
  for (i = 0; i < keylen; ++i) {
    pad[i] ^= key[i];
  }
  keccak_update(&S->outer, pad, KECCAK_BLOCKLEN);

  memwipe(keyhash, HASH_SIZE);
}

void hmac_keccak_update(hmac_keccak_state *S, const uint8_t *data, size_t datalen) {
  keccak_update(&S->inner, data, datalen);
}

void hmac_keccak_finish(hmac_keccak_state *S, uint8_t *digest) {
  uint8_t ihash[HASH_SIZE];
  keccak_finish(&S->inner, ihash);
  keccak_update(&S->outer, ihash, HASH_SIZE);
  keccak_finish(&S->outer, digest);
  memwipe(ihash, HASH_SIZE);
}

void hmac_keccak_hash(uint8_t *out, const uint8_t *key, size_t keylen, const uint8_t *in, size_t inlen) {
  hmac_keccak_state S;
  hmac_keccak_init(&S, key, keylen);
  hmac_keccak_update(&S, in, inlen);
  hmac_keccak_finish(&S, out);
}
