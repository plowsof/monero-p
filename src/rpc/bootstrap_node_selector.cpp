// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

// 

#include "bootstrap_node_selector.h"

#include "crypto/crypto.h"

namespace cryptonote
{
namespace bootstrap_node
{

  void selector_auto::node::handle_result(bool success)
  {
    if (!success)
    {
      fails = std::min(std::numeric_limits<size_t>::max() - 2, fails) + 2;
    }
    else
    {
      fails = std::max(std::numeric_limits<size_t>::min() + 2, fails) - 2;
    }
  }

  void selector_auto::handle_result(const std::string &address, bool success)
  {
    auto &nodes_by_address = m_nodes.get<by_address>();
    const auto it = nodes_by_address.find(address);
    if (it != nodes_by_address.end())
    {
      nodes_by_address.modify(it, [success](node &entry) {
        entry.handle_result(success);
      });
    }
  }

  boost::optional<node_info> selector_auto::next_node()
  {
    if (!has_at_least_one_good_node())
    {
      append_new_nodes();
    }

    if (m_nodes.empty())
    {
      return {};
    }

    auto node = m_nodes.get<by_fails>().begin();
    const size_t count = std::distance(node, m_nodes.get<by_fails>().upper_bound(node->fails));
    std::advance(node, crypto::rand_idx(count));

    return {{node->address, {}}};
  }

  bool selector_auto::has_at_least_one_good_node() const
  {
    return !m_nodes.empty() && m_nodes.get<by_fails>().begin()->fails == 0;
  }

  void selector_auto::append_new_nodes()
  {
    bool updated = false;

    for (const auto &node : m_get_nodes())
    {
      const auto &address = node.first;
      const auto &white = node.second;
      const size_t initial_score = white ? 0 : 1;
      updated |= m_nodes.get<by_address>().insert({address, initial_score}).second;
    }

    if (updated)
    {
      truncate();
    }
  }

  void selector_auto::truncate()
  {
    const size_t total = m_nodes.size();
    if (total > m_max_nodes)
    {
      auto &nodes_by_fails = m_nodes.get<by_fails>();
      auto from = nodes_by_fails.rbegin();
      std::advance(from, total - m_max_nodes);
      nodes_by_fails.erase(from.base(), nodes_by_fails.end());
    }
  }

}
}
