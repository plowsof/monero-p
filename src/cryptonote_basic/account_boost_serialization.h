// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 
// 
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#pragma once

#include "account.h"
#include "cryptonote_boost_serialization.h"

//namespace cryptonote {
namespace boost
{
  namespace serialization
  {
    template <class Archive>
    inline void serialize(Archive &a, cryptonote::account_keys &x, const boost::serialization::version_type ver)
    {
      a & x.m_account_address;
      a & x.m_spend_secret_key;
      a & x.m_view_secret_key;
    }

    template <class Archive>
    inline void serialize(Archive &a, cryptonote::account_public_address &x, const boost::serialization::version_type ver)
    {
      a & x.m_spend_public_key;
      a & x.m_view_public_key;
    }

  }
}
