// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 

#include "rpc_version_str.h"
#include "version.h"
#include <regex>

namespace cryptonote
{

namespace rpc
{

// Expected format of Monero software version string:
// 1) Four numbers, one to two digits each, separated by periods
// 2) Optionally, one of the following suffixes:
//      a) -release
//      b) -<hash> where <hash> is exactly nine lowercase hex digits

bool is_version_string_valid(const std::string& str)
{
    return std::regex_match(str, std::regex(
        "^\\d{1,2}(\\.\\d{1,2}){3}(-(release|[0-9a-f]{9}))?$",
        std::regex_constants::nosubs
    ));
}

}  // namespace rpc

}  // namespace cryptonote
