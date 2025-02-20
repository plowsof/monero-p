// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
//

#pragma once

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/thread/future.hpp>
#include <string>

namespace net
{
namespace socks
{
    //! Primarily for use with `epee::net_utils::http_client`.
    struct connector
    {
        boost::asio::ip::tcp::endpoint proxy_address;

        /*! Creates a new socket, asynchronously connects to `proxy_address`,
            and requests a connection to `remote_host` on `remote_port`. Sets
            socket as closed if `timeout` is reached.

            \return The socket if successful, and exception in the future with
                error otherwise. */
        boost::unique_future<boost::asio::ip::tcp::socket>
            operator()(const std::string& remote_host, const std::string& remote_port, boost::asio::steady_timer& timeout) const;
    };
} // socks
} // net
