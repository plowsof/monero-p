// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
//
//
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers



#include "include_base_utils.h"                     // LOG_PRINT_x
#include "common/util.h"

using namespace std;

namespace Monero {
namespace Utils {

bool isAddressLocal(const std::string &address)
{ 
    try {
        return tools::is_local_address(address);
    } catch (const std::exception &e) {
        MERROR("error: " << e.what());
        return false;
    }
}

void onStartup()
{
    tools::on_startup();
#ifdef NDEBUG
    tools::disable_core_dumps();
#endif
}

}


} // namespace
