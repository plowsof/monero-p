// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

//

#include "net/resolve.h"

#include <boost/utility/string_ref.hpp>
#include "common/dns_utils.h"
#include "common/expect.h"
#include "net/error.h"

namespace net
{
namespace dnssec
{
  expect<service_response> resolve_hostname(const std::string& addr, const std::string& tlsa_port)
  {
    // use basic (blocking) unbound for now, possibly refactor later
    tools::DNSResolver& resolver = tools::DNSResolver::instance();

    bool dnssec_available = false;
    bool dnssec_valid = false;
    std::vector<std::string> ip_records = resolver.get_ipv4(addr, dnssec_available, dnssec_valid);

    if (dnssec_available && !dnssec_valid)
      return {net::error::bogus_dnssec};

    if (ip_records.empty())
    {
      ip_records = resolver.get_ipv6(addr, dnssec_available, dnssec_valid);
      if (dnssec_available && !dnssec_valid)
        return {net::error::bogus_dnssec};
      if (ip_records.empty())
        return {net::error::dns_query_failure};
    }

    std::vector<std::string> tlsa{};
    if (dnssec_available && !tlsa_port.empty())
    {
      tlsa = resolver.get_tlsa_tcp_record(addr, tlsa_port, dnssec_available, dnssec_valid);
      if (!dnssec_valid)
        return {net::error::bogus_dnssec};
    }
    return {{std::move(ip_records), std::move(tlsa)}};
  }
} // dnssec
} // net
