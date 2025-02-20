// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
//

#pragma once

#include <boost/utility/string_ref.hpp>
#include <cstdint>
#include <string>

#include "common/expect.h"
#include "net/enums.h"
#include "net/error.h"

namespace epee
{
namespace serialization
{
    class portable_storage;
    struct section;
}
}

namespace net
{
    //! b32 i2p address; internal format not condensed/decoded.
    class i2p_address
    {
        char host_[61]; // null-terminated

        //! Keep in private, `host.size()` has no runtime check
        i2p_address(boost::string_ref host) noexcept;

    public:
        //! \return Size of internal buffer for host.
        static constexpr std::size_t buffer_size() noexcept { return sizeof(host_); }

        //! \return `<unknown tor host>`.
        static const char* unknown_str() noexcept;

        //! An object with `port() == 0` and `host_str() == unknown_str()`.
        i2p_address() noexcept;

        //! \return A default constructed `i2p_address` object.
        static i2p_address unknown() noexcept { return i2p_address{}; }

        /*!
            Parse `address` in b32 i2p format (i.e. x.b32.i2p:80)
            with `default_port` being used if port is not specified in
            `address`.
        */
        static expect<i2p_address> make(boost::string_ref address);

        //! Load from epee p2p format, and \return false if not valid tor address
        bool _load(epee::serialization::portable_storage& src, epee::serialization::section* hparent);

        //! Store in epee p2p format
        bool store(epee::serialization::portable_storage& dest, epee::serialization::section* hparent) const;

        // Moves and copies are currently identical

        i2p_address(const i2p_address& rhs) noexcept;
        ~i2p_address() = default;
        i2p_address& operator=(const i2p_address& rhs) noexcept;

        //! \return True if default constructed or via `unknown()`.
        bool is_unknown() const noexcept;

        bool equal(const i2p_address& rhs) const noexcept;
        bool less(const i2p_address& rhs) const noexcept;

        //! \return True if i2p addresses are identical.
        bool is_same_host(const i2p_address& rhs) const noexcept;

        //! \return `x.b32.i2p` or `x.b32.i2p:z` if `port() != 0`.
        std::string str() const;

        //! \return Null-terminated `x.b32.i2p` value or `unknown_str()`.
        const char* host_str() const noexcept { return host_; }

        //! \return `1` to work with I2P socks which considers `0` error.
        std::uint16_t port() const noexcept { return 1; }

        static constexpr bool is_loopback() noexcept { return false; }
        static constexpr bool is_local() noexcept { return false; }

        static constexpr epee::net_utils::address_type get_type_id() noexcept
        {
            return epee::net_utils::address_type::i2p;
        }

        static constexpr epee::net_utils::zone get_zone() noexcept
        {
            return epee::net_utils::zone::i2p;
        }

        //! \return `!is_unknown()`.
        bool is_blockable() const noexcept { return !is_unknown(); }
    };

    inline bool operator==(const i2p_address& lhs, const i2p_address& rhs) noexcept
    {
        return lhs.equal(rhs);
    }

    inline bool operator!=(const i2p_address& lhs, const i2p_address& rhs) noexcept
    {
        return !lhs.equal(rhs);
    }

    inline bool operator<(const i2p_address& lhs, const i2p_address& rhs) noexcept
    {
        return lhs.less(rhs);
    }
} // net
