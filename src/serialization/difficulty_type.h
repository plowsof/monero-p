// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 

#pragma once

#include "cryptonote_basic/difficulty.h"
#include "serialization.h"

template <template <bool> class Archive>
inline bool do_serialize(Archive<false>& ar, cryptonote::difficulty_type &diff)
{
  uint64_t hi, lo;
  ar.serialize_varint(hi);
  if (!ar.good())
    return false;
  ar.serialize_varint(lo);
  if (!ar.good())
    return false;
  diff = hi;
  diff <<= 64;
  diff += lo;
  return true;
}

template <template <bool> class Archive>
inline bool do_serialize(Archive<true>& ar, cryptonote::difficulty_type &diff)
{
  if (!ar.good())
    return false;
  const uint64_t hi = ((diff >> 64) & 0xffffffffffffffff).convert_to<uint64_t>();
  const uint64_t lo = (diff & 0xffffffffffffffff).convert_to<uint64_t>();
  ar.serialize_varint(hi);
  ar.serialize_varint(lo);
  if (!ar.good())
    return false;
  return true;
}

