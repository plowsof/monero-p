// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 

#include "gtest/gtest.h"

#include "cryptonote_basic/account.h"

TEST(account, encrypt_keys)
{
  cryptonote::keypair recovery_key = cryptonote::keypair::generate(hw::get_device("default"));
  cryptonote::account_base account;
  crypto::secret_key key = account.generate(recovery_key.sec);
  const cryptonote::account_keys keys = account.get_keys();

  ASSERT_EQ(account.get_keys().m_account_address, keys.m_account_address);
  ASSERT_EQ(account.get_keys().m_spend_secret_key, keys.m_spend_secret_key);
  ASSERT_EQ(account.get_keys().m_view_secret_key, keys.m_view_secret_key);
  ASSERT_EQ(account.get_keys().m_multisig_keys, keys.m_multisig_keys);

  crypto::chacha_key chacha_key;
  crypto::generate_chacha_key(&recovery_key, sizeof(recovery_key), chacha_key, 1);

  account.encrypt_keys(chacha_key);

  ASSERT_EQ(account.get_keys().m_account_address, keys.m_account_address);
  ASSERT_NE(account.get_keys().m_spend_secret_key, keys.m_spend_secret_key);
  ASSERT_NE(account.get_keys().m_view_secret_key, keys.m_view_secret_key);

  account.decrypt_viewkey(chacha_key);

  ASSERT_EQ(account.get_keys().m_account_address, keys.m_account_address);
  ASSERT_NE(account.get_keys().m_spend_secret_key, keys.m_spend_secret_key);
  ASSERT_EQ(account.get_keys().m_view_secret_key, keys.m_view_secret_key);

  account.encrypt_viewkey(chacha_key);

  ASSERT_EQ(account.get_keys().m_account_address, keys.m_account_address);
  ASSERT_NE(account.get_keys().m_spend_secret_key, keys.m_spend_secret_key);
  ASSERT_NE(account.get_keys().m_view_secret_key, keys.m_view_secret_key);

  account.decrypt_keys(chacha_key);

  ASSERT_EQ(account.get_keys().m_account_address, keys.m_account_address);
  ASSERT_EQ(account.get_keys().m_spend_secret_key, keys.m_spend_secret_key);
  ASSERT_EQ(account.get_keys().m_view_secret_key, keys.m_view_secret_key);
}
