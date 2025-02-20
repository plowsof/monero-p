// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

// 

#pragma  once

#include <functional>
#include <limits>
#include <map>
#include <string>
#include <utility>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/optional/optional.hpp>

#include "net/http_client.h"

namespace cryptonote
{
namespace bootstrap_node
{

  struct node_info
  {
    std::string address;
    boost::optional<epee::net_utils::http::login> credentials;
  };

  struct selector
  {
    virtual ~selector() = default;

    virtual void handle_result(const std::string &address, bool success) = 0;
    virtual boost::optional<node_info> next_node() = 0;
  };

  class selector_auto : public selector
  {
  public:
    selector_auto(std::function<std::map<std::string, bool>()> get_nodes, size_t max_nodes = 1000)
      : m_get_nodes(std::move(get_nodes))
      , m_max_nodes(max_nodes)
    {}

    void handle_result(const std::string &address, bool success) final;
    boost::optional<node_info> next_node() final;

  private:
    bool has_at_least_one_good_node() const;
    void append_new_nodes();
    void truncate();

  private:
    struct node
    {
      std::string address;
      size_t fails;

      void handle_result(bool success);
    };

    struct by_address {};
    struct by_fails {};

    typedef boost::multi_index_container<
      node,
      boost::multi_index::indexed_by<
        boost::multi_index::ordered_unique<boost::multi_index::tag<by_address>, boost::multi_index::member<node, std::string, &node::address>>,
        boost::multi_index::ordered_non_unique<boost::multi_index::tag<by_fails>, boost::multi_index::member<node, size_t, &node::fails>>
      >
    > nodes_list;

    const std::function<std::map<std::string, bool>()> m_get_nodes;
    const size_t m_max_nodes;
    nodes_list m_nodes;
  };

}
}
