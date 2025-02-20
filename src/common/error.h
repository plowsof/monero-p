// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <system_error>
#include <type_traits>

enum class common_error : int
{
    // 0 is reserved for no error, as per expect<T>
    kInvalidArgument = 1, //!< A function argument is invalid
    kInvalidErrorCode     //!< Default `std::error_code` given to `expect<T>`
};

std::error_category const& common_category() noexcept;

inline std::error_code make_error_code(::common_error value) noexcept
{
    return std::error_code{int(value), common_category()};
}

namespace std
{
    template<>
    struct is_error_code_enum<::common_error>
      : true_type
    {};
}
