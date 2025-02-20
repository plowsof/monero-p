// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
#pragma once

#include <system_error>
#include <type_traits>

//! Executes a LMDB command, and returns errors via `lmdb::error` enum.
#define MONERO_LMDB_CHECK(...)         \
    do                                 \
    {                                  \
        const int err = __VA_ARGS__ ;  \
        if (err)                       \
            return {lmdb::error(err)}; \
    } while (0)

namespace lmdb
{
    //! Tracks LMDB error codes.
    enum class error : int
    {
        // 0 is reserved for no error, as per expect<T>
        // All other errors are the values reported by LMDB
    };

    std::error_category const& error_category() noexcept;

    inline std::error_code make_error_code(error value) noexcept
    {
        return std::error_code{int(value), error_category()};
    }
}

namespace std
{
    template<>
    struct is_error_code_enum<::lmdb::error>
      : true_type
    {};
}
