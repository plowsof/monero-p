// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 
// 
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#pragma once

#include <boost/uuid/uuid.hpp>
#include <utility>
#include <vector>
#include "cryptonote_basic/blobdatatype.h"
#include "cryptonote_protocol/enums.h"
#include "cryptonote_protocol/fwd.h"
#include "net/enums.h"
#include "net/net_utils_base.h"
#include "p2p_protocol_defs.h"

namespace epee { namespace levin { class message_writer; } }

namespace nodetool
{

  typedef boost::uuids::uuid uuid;
  typedef boost::uuids::uuid net_connection_id;

  template<class t_connection_context>
  struct i_p2p_endpoint
  {
    virtual bool relay_notify_to_list(int command, epee::levin::message_writer message, std::vector<std::pair<epee::net_utils::zone, boost::uuids::uuid>> connections)=0;
    virtual epee::net_utils::zone send_txs(std::vector<cryptonote::blobdata> txs, const epee::net_utils::zone origin, const boost::uuids::uuid& source, cryptonote::relay_method tx_relay)=0;
    virtual bool invoke_notify_to_peer(int command, epee::levin::message_writer message, const epee::net_utils::connection_context_base& context)=0;
    virtual bool drop_connection(const epee::net_utils::connection_context_base& context)=0;
    virtual void request_callback(const epee::net_utils::connection_context_base& context)=0;
    virtual uint64_t get_public_connections_count()=0;
    virtual void for_each_connection(std::function<bool(t_connection_context&, peerid_type, uint32_t)> f)=0;
    virtual bool for_connection(const boost::uuids::uuid&, std::function<bool(t_connection_context&, peerid_type, uint32_t)> f)=0;
    virtual bool block_host(epee::net_utils::network_address address, time_t seconds = 0, bool add_only = false)=0;
    virtual bool unblock_host(const epee::net_utils::network_address &address)=0;
    virtual std::map<std::string, time_t> get_blocked_hosts()=0;
    virtual std::map<epee::net_utils::ipv4_network_subnet, time_t> get_blocked_subnets()=0;
    virtual bool add_host_fail(const epee::net_utils::network_address &address, unsigned int score = 1)=0;
    virtual void add_used_stripe_peer(const t_connection_context &context)=0;
    virtual void remove_used_stripe_peer(const t_connection_context &context)=0;
    virtual void clear_used_stripe_peers()=0;
  };

  template<class t_connection_context>
  struct p2p_endpoint_stub: public i_p2p_endpoint<t_connection_context>
  {
    virtual bool relay_notify_to_list(int command, epee::levin::message_writer message, std::vector<std::pair<epee::net_utils::zone, boost::uuids::uuid>> connections)
    {
      return false;
    }
    virtual epee::net_utils::zone send_txs(std::vector<cryptonote::blobdata> txs, const epee::net_utils::zone origin, const boost::uuids::uuid& source, cryptonote::relay_method tx_relay)
    {
      return epee::net_utils::zone::invalid;
    }
    virtual bool invoke_notify_to_peer(int command, epee::levin::message_writer message, const epee::net_utils::connection_context_base& context)
    {
      return true;
    }
    virtual bool drop_connection(const epee::net_utils::connection_context_base& context)
    {
      return false;
    }
    virtual void request_callback(const epee::net_utils::connection_context_base& context)
    {

    }
    virtual void for_each_connection(std::function<bool(t_connection_context&,peerid_type,uint32_t)> f)
    {

    }
    virtual bool for_connection(const boost::uuids::uuid&, std::function<bool(t_connection_context&,peerid_type,uint32_t)> f)
    {
      return false;
    }

    virtual uint64_t get_public_connections_count()    
    {
      return false;
    }
    virtual bool block_host(epee::net_utils::network_address address, time_t seconds, bool add_only)
    {
      return true;
    }
    virtual bool unblock_host(const epee::net_utils::network_address &address)
    {
      return true;
    }
    virtual std::map<std::string, time_t> get_blocked_hosts()
    {
      return std::map<std::string, time_t>();
    }
    virtual std::map<epee::net_utils::ipv4_network_subnet, time_t> get_blocked_subnets()
    {
      return std::map<epee::net_utils::ipv4_network_subnet, time_t>();
    }
    virtual bool add_host_fail(const epee::net_utils::network_address &address, unsigned int score)
    {
      return true;
    }
    virtual void add_used_stripe_peer(const t_connection_context &context)
    {
    }
    virtual void remove_used_stripe_peer(const t_connection_context &context)
    {
    }
    virtual void clear_used_stripe_peers()
    {
    }
  };
}
