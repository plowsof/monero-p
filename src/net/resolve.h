// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

//

#include <string>
#include <vector>

template<typename> class expect;

namespace net
{
namespace dnssec
{
  struct service_response
  {
    std::vector<std::string> ip;   //!< IPv4/6 records in dotted or semicolon notation
    std::vector<std::string> tlsa; //!< DANE/TLSA records
  };

  //! \return IP + (optionally) DANE/TLSA records, failing if DNSSEC signature is "bogus"
  expect<service_response> resolve_hostname(const std::string& addr, const std::string& tlsa_port = {});
} // dnssec
} // net
