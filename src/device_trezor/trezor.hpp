// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
//
//

#ifndef MONERO_TREZOR_HPP
#define MONERO_TREZOR_HPP

#include "trezor/trezor_defs.hpp"

#ifdef WITH_DEVICE_TREZOR
#include "trezor/transport.hpp"
#include "trezor/messages/messages.pb.h"
#include "trezor/messages/messages-common.pb.h"
#include "trezor/messages/messages-management.pb.h"
#include "trezor/messages/messages-monero.pb.h"
#include "trezor/protocol.hpp"
#endif

#endif //MONERO_TREZOR_HPP
