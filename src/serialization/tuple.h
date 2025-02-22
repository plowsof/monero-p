// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 
// 
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#pragma once
#include <tuple>
#include "serialization.h"

namespace serialization
{
  namespace detail
  {
    template <typename Archive, class T>
    bool serialize_tuple_element(Archive& ar, T& e)
    {
      return do_serialize(ar, e);
    }

    template <typename Archive>
    bool serialize_tuple_element(Archive& ar, uint64_t& e)
    {
      ar.serialize_varint(e);
      return true;
    }
  }
}

template <size_t I, bool BackwardsCompat, bool W, template <bool> class Archive, typename... Ts>
bool do_serialize_tuple_nth(Archive<W>& ar, std::tuple<Ts...>& v)
{
  static constexpr const size_t tuple_size = std::tuple_size<std::tuple<Ts...>>();
  static_assert(I <= tuple_size, "bad call");

  if constexpr (I == 0)
  {
    // If BackwardsCompat is true, we serialize the size of 3-tuples and 4-tuples
    if constexpr (BackwardsCompat && (tuple_size == 3 || tuple_size == 4))
    {
      size_t cnt = tuple_size;
      ar.begin_array(cnt);
      if (cnt != tuple_size)
        return false;
    }
    else
    {
      ar.begin_array();
    }
  }
  else if constexpr (I < tuple_size)
  {
    ar.delimit_array();
  }

  if constexpr (I == tuple_size)
  {
    ar.end_array();
    return ar.good();
  }
  else
  {
    if (!::serialization::detail::serialize_tuple_element(ar, std::get<I>(v))
        || !ar.good())
      return false;

    return do_serialize_tuple_nth<I + 1, BackwardsCompat>(ar, v);
  }
}

template <bool BackwardsCompat, bool W, template <bool> class Archive, typename... Ts>
bool do_serialize_tuple(Archive<W>& ar, std::tuple<Ts...>& v)
{
  return do_serialize_tuple_nth<0, BackwardsCompat>(ar, v);
}

template <bool W, template <bool> class Archive, typename... Ts>
bool do_serialize(Archive<W>& ar, std::tuple<Ts...>& v)
{
  return do_serialize_tuple<true>(ar, v);
}

#define TUPLE_COMPACT_FIELDS(v)                          \
  do {                                                   \
    if (!do_serialize_tuple<false>(ar, v) || !ar.good()) \
      return false;                                      \
  } while (0);

#define TUPLE_COMPACT_FIELD_N(t, v) \
  do {                              \
    ar.tag(t);                      \
    TUPLE_COMPACT_FIELDS(v);        \
  } while (0);

#define TUPLE_COMPACT_FIELD(f) TUPLE_COMPACT_FIELD_N(#f, f)

#define TUPLE_COMPACT_FIELD_F(f) TUPLE_COMPACT_FIELD_N(#f, v.f)
