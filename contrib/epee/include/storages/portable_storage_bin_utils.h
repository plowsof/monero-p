// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 

#pragma once 

#include "int-util.h"

template<typename T> T convert_swapper(T t) { return t; }
template<> inline uint16_t convert_swapper(uint16_t t) { return SWAP16LE(t); }
template<> inline int16_t convert_swapper(int16_t t) { return SWAP16LE((uint16_t&)t); }
template<> inline uint32_t convert_swapper(uint32_t t) { return SWAP32LE(t); }
template<> inline int32_t convert_swapper(int32_t t) { return SWAP32LE((uint32_t&)t); }
template<> inline uint64_t convert_swapper(uint64_t t) { return SWAP64LE(t); }
template<> inline int64_t convert_swapper(int64_t t) { return SWAP64LE((uint64_t&)t); }
template<> inline double convert_swapper(double t) { union { uint64_t u; double d; } u; u.d = t; u.u = SWAP64LE(u.u); return u.d; }

#if BYTE_ORDER == BIG_ENDIAN
#define CONVERT_POD(x) convert_swapper(x)
#else
#define CONVERT_POD(x) (x)
#endif
