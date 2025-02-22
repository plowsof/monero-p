// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

//

#pragma once

#include <system_error>
#include <type_traits>

namespace net
{
    //! General net errors
    enum class error : int
    {
        // 0 reserved for success (as per expect<T>)
        bogus_dnssec = 1,   //!< Invalid response signature from DNSSEC enabled domain
        dns_query_failure,  //!< Failed to retrieve desired DNS record
        expected_tld,       //!< Expected a tld
        invalid_host,       //!< Hostname is not valid
        invalid_i2p_address,
        invalid_mask,       //!< Outside of 0-32 range
        invalid_port,       //!< Outside of 0-65535 range
        invalid_tor_address,//!< Invalid base32 or length
        unsupported_address,//!< Type not supported by `get_network_address`

    };

    //! \return `std::error_category` for `net` namespace.
    std::error_category const& error_category() noexcept;

    //! \return `net::error` as a `std::error_code` value.
    inline std::error_code make_error_code(error value) noexcept
    {
        return std::error_code{int(value), error_category()};
    }
}

namespace std
{
    template<>
    struct is_error_code_enum<::net::error>
      : true_type
    {};
}
