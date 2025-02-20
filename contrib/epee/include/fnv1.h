// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

// 

#pragma once

namespace epee
{

namespace fnv
{
  inline uint64_t FNV1a(const char *ptr, size_t sz)
  {
    uint64_t h = 0xcbf29ce484222325;
    for (size_t i = 0; i < sz; ++i)
      h = (h ^ *(const uint8_t*)ptr++) * 0x100000001b3;
    return h;
  }
}

}
