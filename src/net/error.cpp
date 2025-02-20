// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

//

#include "error.h"

#include <string>

namespace
{
    struct net_category : std::error_category
    {
        net_category() noexcept
          : std::error_category()
        {}

        const char* name() const noexcept override
        {
            return "net::error_category";
        }

        std::string message(int value) const override
        {
            switch (net::error(value))
            {
            case net::error::bogus_dnssec:
                return "Invalid response signature from DNSSEC enabled domain";
            case net::error::dns_query_failure:
                return "Failed to retrieve desired DNS record";
            case net::error::expected_tld:
                return "Expected top-level domain";
            case net::error::invalid_host:
                return "Host value is not valid";
            case net::error::invalid_i2p_address:
                return "Invalid I2P address";
            case net::error::invalid_mask:
                return "CIDR netmask outside of 0-32 range";
            case net::error::invalid_port:
                return "Invalid port value (expected 0-65535)";
            case net::error::invalid_tor_address:
                return "Invalid Tor address";
            case net::error::unsupported_address:
                return "Network address not supported";
            default:
                break;
            }

            return "Unknown net::error";
        }

        std::error_condition default_error_condition(int value) const noexcept override
        {
            switch (net::error(value))
            {
            case net::error::invalid_port:
            case net::error::invalid_mask:
                return std::errc::result_out_of_range;
            case net::error::expected_tld:
            case net::error::invalid_tor_address:
            default:
                break;
            }
            return std::error_condition{value, *this};
        }
    };
} // anonymous

namespace net
{
    std::error_category const& error_category() noexcept
    {
        static const net_category instance{};
        return instance;
    }
}
