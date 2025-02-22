// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 

#pragma once

namespace cryptonote
{

namespace rpc
{

static const uint32_t DAEMON_RPC_VERSION_ZMQ_MINOR = 0;
static const uint32_t DAEMON_RPC_VERSION_ZMQ_MAJOR = 2;

static const uint32_t DAEMON_RPC_VERSION_ZMQ = DAEMON_RPC_VERSION_ZMQ_MINOR + (DAEMON_RPC_VERSION_ZMQ_MAJOR << 16);

}  // namespace rpc

}  // namespace cryptonote
