// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <type_traits>

//! Declare an enum to be serialized as an integer
#define WIRE_AS_INTEGER(type_)                                          \
  static_assert(std::is_enum<type_>(), "AS_INTEGER only enum types");   \
  template<typename R>                                                  \
  inline void read_bytes(R& source, type_& dest)                        \
  {                                                                     \
    std::underlying_type<type_>::type temp{};                           \
    read_bytes(source, temp);                                           \
    dest = type_(temp);                                                 \
  }                                                                     \
  template<typename W>                                                  \
  inline void write_bytes(W& dest, const type_ source)                  \
  { write_bytes(dest, std::underlying_type<type_>::type(source)); }

//! Declare functions that list fields in `type` (using virtual interface)
#define WIRE_DECLARE_OBJECT(type)                       \
  void read_bytes(::wire::reader&, type&);              \
  void write_bytes(::wire::writer&, const type&)

//! Cast readers to `rtype` and writers to `wtype` before code expansion
#define WIRE_BEGIN_MAP_BASE(rtype, wtype)				\
  template<typename R>                                                  \
  void read_bytes(R& source)                                            \
  { wire_map(std::true_type{}, static_cast<rtype&>(source), *this); }	\
                                                                        \
  template<typename W>                                                  \
  void write_bytes(W& dest) const                                       \
  { wire_map(std::false_type{}, static_cast<wtype&>(dest), *this); }	\
                                                                        \
  template<typename B, typename F, typename T>                          \
  static void wire_map(const B is_read, F& format, T& self)             \
  { ::wire::object_fwd(is_read, format
  
/*! Define `read_bytes`, and `write_bytes` for `this` that forward the
  derived format types for max performance. */
#define WIRE_BEGIN_MAP()			\
  WIRE_BEGIN_MAP_BASE(R, W)

/*! Define `read_bytes`, and `write_bytes` for `this` that forward base format
  types to reduce code expansion and executable size. */
#define WIRE_BEGIN_MAP_ASM_SIZE()			\
  WIRE_BEGIN_MAP_BASE(::wire::reader, ::wire::writer)

//! End object map; omit last `,`
#define WIRE_END_MAP() );}

namespace wire
{
  struct basic_value;
  class reader;
  struct writer;

  // defined in `wire/read.h`
  template<typename R, typename... T>
  void object_fwd(std::true_type is_read, R& source, T&&... fields);

  // defined in `wire/write.h`
  template<typename W, typename... T>
  void object_fwd(std::false_type is_read, W& dest, T... fields);
}
namespace wire_read
{
  // defined in `wire/read.h`
  template<typename R, typename T>
  void bytes(R& source, T&& dest);
}
namespace wire_write
{
  // defined in `wire/write.h`
  template<typename W, typename T>
  void bytes(W& dest, const T& source);
}
