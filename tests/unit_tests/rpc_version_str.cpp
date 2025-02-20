// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 

#include "gtest/gtest.h"

#include "rpc/rpc_version_str.h"
#include "version.h"

TEST(rpc, is_version_string_valid)
{
  using namespace cryptonote::rpc;
  ASSERT_TRUE(is_version_string_valid(MONERO_VERSION));
  ASSERT_TRUE(is_version_string_valid("0.14.1.2"));
  ASSERT_TRUE(is_version_string_valid("0.15.0.0-release"));
  ASSERT_TRUE(is_version_string_valid("0.15.0.0-fe3f6a3e6"));

  ASSERT_FALSE(is_version_string_valid(""));
  ASSERT_FALSE(is_version_string_valid("invalid"));
  ASSERT_FALSE(is_version_string_valid("0.15.0.0-invalid"));
  ASSERT_FALSE(is_version_string_valid("0.15.0.0-release0"));
  ASSERT_FALSE(is_version_string_valid("0.15.0.0-release "));
  ASSERT_FALSE(is_version_string_valid("0.15.0.0-fe3f6a3e60"));
  ASSERT_FALSE(is_version_string_valid("0.15.0.0-fe3f6a3e6 "));
}
