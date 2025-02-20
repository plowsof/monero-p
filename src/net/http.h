// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

//

#pragma once

#include "net/http_client.h"

namespace net
{
namespace http
{

class client : public epee::net_utils::http::http_simple_client
{
public:
  bool set_proxy(const std::string &address) override;
};

class client_factory : public epee::net_utils::http::http_client_factory
{
public:
  std::unique_ptr<epee::net_utils::http::abstract_http_client> create() override;
};

} // namespace http
} // namespace net
