// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
//
//
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#include "p2p/net_node.h"
#include "p2p/net_node.inl"
#include "cryptonote_protocol/cryptonote_protocol_handler.h"
#include "cryptonote_protocol/cryptonote_protocol_handler.inl"

namespace nodetool { template class node_server<cryptonote::t_cryptonote_protocol_handler<cryptonote::core>>; }
namespace cryptonote { template class t_cryptonote_protocol_handler<cryptonote::core>; }
