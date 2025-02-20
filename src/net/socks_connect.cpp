// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
//

#include "socks_connect.h"

#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <cstdint>
#include <memory>
#include <system_error>

#include "net/error.h"
#include "net/net_utils_base.h"
#include "net/socks.h"
#include "string_tools.h"
#include "string_tools_lexical.h"

namespace net
{
namespace socks
{
    boost::unique_future<boost::asio::ip::tcp::socket>
    connector::operator()(const std::string& remote_host, const std::string& remote_port, boost::asio::steady_timer& timeout) const
    {
        struct future_socket
        {
            boost::promise<boost::asio::ip::tcp::socket> result_;

            void operator()(boost::system::error_code error, boost::asio::ip::tcp::socket&& socket)
            {
                if (error)
                    result_.set_exception(boost::system::system_error{error});
                else
                    result_.set_value(std::move(socket));
            }
        };

        boost::unique_future<boost::asio::ip::tcp::socket> out{};
        {
            std::uint16_t port = 0;
            if (!epee::string_tools::get_xtype_from_string(port, remote_port))
                throw std::system_error{net::error::invalid_port, "Remote port for socks proxy"};

            bool is_set = false;
            std::uint32_t ip_address = 0;
            boost::promise<boost::asio::ip::tcp::socket> result{};
            out = result.get_future();
            const auto proxy = net::socks::make_connect_client(
                boost::asio::ip::tcp::socket{MONERO_GET_EXECUTOR(timeout)}, net::socks::version::v4a, future_socket{std::move(result)}
            );

            if (epee::string_tools::get_ip_int32_from_string(ip_address, remote_host))
                is_set = proxy->set_connect_command(epee::net_utils::ipv4_network_address{ip_address, port});
            else
                is_set = proxy->set_connect_command(remote_host, port);

            if (!is_set || !net::socks::client::connect_and_send(proxy, proxy_address))
                throw std::system_error{net::error::invalid_host, "Address for socks proxy"};

            timeout.async_wait(net::socks::client::async_close{std::move(proxy)});
        }

        return out;
    }
} // socks
} // net
