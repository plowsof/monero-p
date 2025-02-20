// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 
//
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#include "gtest/gtest.h"

#ifdef STATICLIB

extern "C" int dnskey_algo_id_is_supported(int);

TEST(unbound, supported_algorithms)
{
  // Monero causes these to be tried, but we don't have access
  // to this internal unbound header here, so we use raw numbers
  // LDNS_RSASHA1            = 5,
  // LDNS_RSASHA1_NSEC3      = 7,
  // LDNS_RSASHA256          = 8,   /* RFC 5702 */
  // LDNS_ECDSAP256SHA256    = 13,  /* RFC 6605 */

  ASSERT_TRUE(dnskey_algo_id_is_supported(5));
  ASSERT_TRUE(dnskey_algo_id_is_supported(7));
  ASSERT_TRUE(dnskey_algo_id_is_supported(8));
  ASSERT_TRUE(dnskey_algo_id_is_supported(13));
}

#endif

