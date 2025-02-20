// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 
// 
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#pragma once

#include <cstddef>
#include <cstring>
#include <functional>
#include <memory>
#include <sodium/crypto_verify_32.h>

#define CRYPTO_MAKE_COMPARABLE(type) \
namespace crypto { \
  inline bool operator==(const type &_v1, const type &_v2) { \
    return !memcmp(&_v1, &_v2, sizeof(_v1)); \
  } \
  inline bool operator!=(const type &_v1, const type &_v2) { \
    return !operator==(_v1, _v2); \
  } \
}

#define CRYPTO_MAKE_COMPARABLE_CONSTANT_TIME(type) \
namespace crypto { \
  inline bool operator==(const type &_v1, const type &_v2) { \
    static_assert(sizeof(_v1) == 32, "constant time comparison is only implenmted for 32 bytes"); \
    return crypto_verify_32((const unsigned char*)&_v1, (const unsigned char*)&_v2) == 0; \
  } \
  inline bool operator!=(const type &_v1, const type &_v2) { \
    return !operator==(_v1, _v2); \
  } \
}

#define CRYPTO_DEFINE_HASH_FUNCTIONS(type) \
namespace crypto { \
  static_assert(sizeof(std::size_t) <= sizeof(type), "Size of " #type " must be at least that of size_t"); \
  inline std::size_t hash_value(const type &_v) { \
    std::size_t h; \
    memcpy(&h, std::addressof(_v), sizeof(h)); \
    return h; \
  } \
} \
namespace std { \
  template<> \
  struct hash<crypto::type> { \
    std::size_t operator()(const crypto::type &_v) const { \
      std::size_t h; \
      memcpy(&h, std::addressof(_v), sizeof(h)); \
      return h; \
    } \
  }; \
}

#define CRYPTO_MAKE_HASHABLE(type) \
CRYPTO_MAKE_COMPARABLE(type) \
CRYPTO_DEFINE_HASH_FUNCTIONS(type)

#define CRYPTO_MAKE_HASHABLE_CONSTANT_TIME(type) \
CRYPTO_MAKE_COMPARABLE_CONSTANT_TIME(type) \
CRYPTO_DEFINE_HASH_FUNCTIONS(type)

