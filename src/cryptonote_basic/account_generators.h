// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 

#pragma once

#include "crypto/crypto.h"
#include "crypto/generators.h"


namespace cryptonote
{

enum class account_generator_era : unsigned char
{
  unknown = 0,
  cryptonote = 1  //and ringct
};

struct account_generators
{
  crypto::public_key m_primary;    //e.g. for spend key
  crypto::public_key m_secondary;  //e.g. for view key
};

inline crypto::public_key get_primary_generator(const account_generator_era era)
{
  if (era == account_generator_era::cryptonote)
    return crypto::get_G();
  else
    return crypto::null_pkey;  //error
}

inline crypto::public_key get_secondary_generator(const account_generator_era era)
{
  if (era == account_generator_era::cryptonote)
    return crypto::get_G();
  else
    return crypto::null_pkey;  //error
}

inline account_generators get_account_generators(const account_generator_era era)
{
  return account_generators{get_primary_generator(era), get_secondary_generator(era)};
}

} //namespace cryptonote
