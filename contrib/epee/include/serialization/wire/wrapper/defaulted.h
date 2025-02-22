// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <functional>
#include <utility>

#include "serialization/wire/field.h"
#include "serialization/wire/traits.h"

//! An optional field that is omitted when a default value is used
#define WIRE_FIELD_DEFAULTED(name, default_)                            \
  ::wire::optional_field( #name , ::wire::defaulted(std::ref( self . name ), default_ ))

namespace wire
{
  /*! A wrapper that tells `wire::writer`s to skip field generation when default
    value, and tells `wire::reader`s to use default value when field not present. */
  template<typename T, typename U>
  struct defaulted_
  {
    using value_type = unwrap_reference_t<T>;

    T value;
    U default_;

    constexpr const value_type& get_value() const noexcept { return value; }
    value_type& get_value() noexcept { return value; }

    // concept requirements for optional fields

    constexpr explicit operator bool() const { return get_value() != default_; }
    value_type& emplace() noexcept { return get_value(); }

    constexpr const value_type& operator*() const noexcept { return get_value(); }
    value_type& operator*() noexcept { return get_value(); }

    void reset() { get_value() = default_; }
  };

  //! Links `value` with `default_`.
  template<typename T, typename U>
  inline constexpr defaulted_<T, U> defaulted(T value, U default_)
  {
    return {std::move(value), std::move(default_)};
  }

  /* read/write functions not needed since `defaulted_` meets the concept
     requirements for an optional type (optional fields are handled
     directly by the generic read/write code because the field name is omitted
     entirely when the value is "empty"). */
} // wire
