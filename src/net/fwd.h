// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

//

#pragma once

#include <cstdint>

namespace net
{
    enum class error : int;
    class tor_address;
    class i2p_address;

    namespace socks
    {
        class client;
        template<typename> class connect_handler;
        enum class error : int;
        enum class version : std::uint8_t;
    }
}
