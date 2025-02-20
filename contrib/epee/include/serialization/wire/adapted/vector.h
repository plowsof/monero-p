// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <cstdint>
#include <cstring>
#include <type_traits>
#include <vector>

#include "byte_slice.h"
#include "serialization/wire/traits.h"

namespace wire
{
  template<typename T, typename A>
  struct is_array<std::vector<T, A>>
    : std::true_type
  {};
  template<typename A>
  struct is_array<std::vector<std::uint8_t, A>>
    : std::false_type
  {};

  template<typename R, typename A>
  inline void read_bytes(R& source, std::vector<std::uint8_t, A>& dest)
  {
    const epee::byte_slice bytes = source.binary();
    dest.resize(bytes.size());
    std::memcpy(dest.data(), bytes.data(), bytes.size());
  }
  template<typename W, typename A>
  inline void write_bytes(W& dest, const std::vector<std::uint8_t, A>& source)
  {
    dest.binary(epee::to_span(source));
  }
}
