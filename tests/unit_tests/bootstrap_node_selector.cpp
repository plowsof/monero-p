// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

// 

#include <gtest/gtest.h>

#include "rpc/bootstrap_node_selector.h"

class bootstrap_node_selector : public ::testing::Test
{
protected:
  void SetUp() override
  {
    nodes.insert(white_nodes.begin(), white_nodes.end());
    nodes.insert(gray_nodes.begin(), gray_nodes.end());
  }

  const std::map<std::string, bool> white_nodes = {
    {
      "white_node_1:18089", true
    },
    {
      "white_node_2:18081", true
    }
  };
  const std::map<std::string, bool> gray_nodes = {
    {
      "gray_node_1:18081", false
    },
    {
      "gray_node_2:18089", false
    }
  };

  std::map<std::string, bool> nodes;
};

TEST_F(bootstrap_node_selector, selector_auto_empty)
{
  cryptonote::bootstrap_node::selector_auto selector([]() {
    return std::map<std::string, bool>();
  });

  EXPECT_FALSE(selector.next_node());
}

TEST_F(bootstrap_node_selector, selector_auto_no_credentials)
{
  cryptonote::bootstrap_node::selector_auto selector([this]() {
    return nodes;
  });

  for (size_t fails = 0; fails < nodes.size(); ++fails)
  {
    const auto current = selector.next_node();
    EXPECT_FALSE(current->credentials);

    selector.handle_result(current->address, false);
  }
}

TEST_F(bootstrap_node_selector, selector_auto_success)
{
  cryptonote::bootstrap_node::selector_auto selector([this]() {
    return nodes;
  });

  auto current = selector.next_node();
  for (size_t fails = 0; fails < nodes.size(); ++fails)
  {
    selector.handle_result(current->address, true);

    current = selector.next_node();
    EXPECT_TRUE(white_nodes.count(current->address) > 0);
  }
}

TEST_F(bootstrap_node_selector, selector_auto_failure)
{
  cryptonote::bootstrap_node::selector_auto selector([this]() {
    return nodes;
  });

  auto current = selector.next_node();
  for (size_t fails = 0; fails < nodes.size(); ++fails)
  {
    const auto previous = current;

    selector.handle_result(current->address, false);

    current = selector.next_node();
    EXPECT_NE(current->address, previous->address);
  }
}

TEST_F(bootstrap_node_selector, selector_auto_white_nodes_first)
{
  cryptonote::bootstrap_node::selector_auto selector([this]() {
    return nodes;
  });

  for (size_t iterations = 0; iterations < 2; ++iterations)
  {
    for (size_t fails = 0; fails < white_nodes.size(); ++fails)
    {
      const auto current = selector.next_node();
      EXPECT_TRUE(white_nodes.count(current->address) > 0);

      selector.handle_result(current->address, false);
    }

    for (size_t fails = 0; fails < gray_nodes.size(); ++fails)
    {
      const auto current = selector.next_node();
      EXPECT_TRUE(gray_nodes.count(current->address) > 0);

      selector.handle_result(current->address, false);
    }
  }
}

TEST_F(bootstrap_node_selector, selector_auto_max_nodes)
{
  const size_t max_nodes = nodes.size() / 2;

  bool populated_once = false;
  cryptonote::bootstrap_node::selector_auto selector([this, &populated_once]() {
    if (!populated_once)
    {
      populated_once = true;
      return nodes;
    }

    return std::map<std::string, bool>();
  }, max_nodes);

  std::set<std::string> unique_nodes;

  for (size_t fails = 0; fails < nodes.size(); ++fails)
  {
    const auto current = selector.next_node();
    unique_nodes.insert(current->address);

    selector.handle_result(current->address, false);
  }

  EXPECT_EQ(unique_nodes.size(), max_nodes);
}
