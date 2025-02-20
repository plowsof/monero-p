// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 
// 
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#pragma once

#include "cryptonote_protocol/cryptonote_protocol_defs.h"
#include "cryptonote_protocol/enums.h"
#include "cryptonote_basic/connection_context.h"
namespace cryptonote
{
  /************************************************************************/
  /*                                                                      */
  /************************************************************************/
  struct i_cryptonote_protocol
  {
    virtual bool is_synchronized() const = 0;
    virtual bool relay_block(NOTIFY_NEW_BLOCK::request& arg, cryptonote_connection_context& exclude_context)=0;
    virtual bool relay_transactions(NOTIFY_NEW_TRANSACTIONS::request& arg, const boost::uuids::uuid& source, epee::net_utils::zone zone, relay_method tx_relay)=0;
    //virtual bool request_objects(NOTIFY_REQUEST_GET_OBJECTS::request& arg, cryptonote_connection_context& context)=0;
  };

  /************************************************************************/
  /*                                                                      */
  /************************************************************************/
  struct cryptonote_protocol_stub: public i_cryptonote_protocol
  {
    virtual bool is_synchronized() const final
    {
      return false;
    }
    virtual bool relay_block(NOTIFY_NEW_BLOCK::request& arg, cryptonote_connection_context& exclude_context)
    {
      return false;
    }
    virtual bool relay_transactions(NOTIFY_NEW_TRANSACTIONS::request& arg, const boost::uuids::uuid& source, epee::net_utils::zone zone, relay_method tx_relay)
    {
      return false;
    }

  };
}
