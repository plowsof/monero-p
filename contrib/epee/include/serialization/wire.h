// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include "serialization/wire/fwd.h"
#include "serialization/wire/read.h"
#include "serialization/wire/write.h"

//! Define functions that list fields in `type` (using virtual interface)
#define WIRE_DEFINE_OBJECT(type, map)                          \
  void read_bytes(::wire::reader& source, type& dest)          \
  { map(source, dest); }                                       \
                                                               \
  void write_bytes(::wire::writer& dest, const type& source)   \
  { map(dest, source); }

//! Define `from_bytes` and `to_bytes` for `this`.
#define WIRE_DEFINE_CONVERSIONS()                                       \
  template<typename R, typename T>                                      \
  std::error_code from_bytes(T&& source)                                \
  { return ::wire_read::from_bytes<R>(std::forward<T>(source), *this); } \
                                                                        \
  template<typename W, typename T>                                      \
  std::error_code to_bytes(T& dest) const                               \
  { return ::wire_write::to_bytes<W>(dest, *this); }

