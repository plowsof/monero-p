// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <list>
#include <type_traits>

#include "serialization/wire/traits.h"

namespace wire
{
  template<typename T, typename A>
  struct is_array<std::list<T, A>>
    : std::true_type
  {};
}
