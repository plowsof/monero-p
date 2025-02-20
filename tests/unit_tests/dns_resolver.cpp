// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 

#include <iostream>
#include <vector>

#include "gtest/gtest.h"

#include "common/dns_utils.h"

TEST(DNSResolver, IPv4Success)
{
  tools::DNSResolver resolver = tools::DNSResolver::create();

  bool avail, valid;

  auto ips = resolver.get_ipv4("example.com", avail, valid);

  ASSERT_LE(1, ips.size());

  ips = tools::DNSResolver::instance().get_ipv4("example.com", avail, valid);

  ASSERT_LE(1, ips.size());
}

TEST(DNSResolver, IPv4Failure)
{
  // guaranteed by IANA/ICANN/RFC to be invalid
  tools::DNSResolver resolver = tools::DNSResolver::create();

  bool avail, valid;

  auto ips = resolver.get_ipv4("example.invalid", avail, valid);

  ASSERT_EQ(0, ips.size());

  ips = tools::DNSResolver::instance().get_ipv4("example.invalid", avail, valid);

  ASSERT_EQ(0, ips.size());
}

TEST(DNSResolver, DNSSECSuccess)
{
  tools::DNSResolver resolver = tools::DNSResolver::create();

  bool avail, valid;

  auto ips = resolver.get_ipv4("example.com", avail, valid);

  ASSERT_LE(1, ips.size());

  ASSERT_TRUE(avail);
  ASSERT_TRUE(valid);
}

TEST(DNSResolver, DNSSECFailure)
{
  tools::DNSResolver resolver = tools::DNSResolver::create();

  bool avail, valid;

  auto ips = resolver.get_ipv4("dnssec-failed.org", avail, valid);

  ASSERT_EQ(1, ips.size());

  //ASSERT_STREQ("93.184.216.119", ips[0].c_str());

  ASSERT_TRUE(avail);
  ASSERT_FALSE(valid);
}

// It would be great to include an IPv6 test and assume it'll pass, but not every ISP / resolver plays nicely with IPv6;)
/*TEST(DNSResolver, IPv6Success)
{
  tools::DNSResolver resolver = tools::DNSResolver::create();

  bool avail, valid;

  auto ips = resolver.get_ipv6("example.com", avail, valid);

  ASSERT_EQ(1, ips.size());

  ASSERT_STREQ("2606:2800:220:6d:26bf:1447:1097:aa7", ips[0].c_str());

  ips = tools::DNSResolver::instance().get_ipv6("example.com", avail, valid);

  ASSERT_EQ(1, ips.size());

  ASSERT_STREQ("2606:2800:220:6d:26bf:1447:1097:aa7", ips[0].c_str());
}*/

TEST(DNSResolver, IPv6Failure)
{
  // guaranteed by IANA/ICANN/RFC to be invalid
  tools::DNSResolver resolver = tools::DNSResolver::create();

  bool avail, valid;

  auto ips = resolver.get_ipv6("example.invalid", avail, valid);

  ASSERT_EQ(0, ips.size());

  ips = tools::DNSResolver::instance().get_ipv6("example.invalid", avail, valid);

  ASSERT_EQ(0, ips.size());
}

TEST(DNSResolver, GetTXTRecord)
{
  bool avail, valid;

  std::vector<std::string> records = tools::DNSResolver::instance().get_txt_record("donate.getmonero.org", avail, valid);

  EXPECT_NE(0, records.size());

  for (auto& rec : records)
  {
    std::cout << "TXT record for donate.getmonero.org: " << rec << std::endl;
  }

  // replace first @ with .
  std::string addr = tools::DNSResolver::instance().get_dns_format_from_oa_address("donate@getmonero.org");
  EXPECT_STREQ("donate.getmonero.org", addr.c_str());

  // no change
  addr = tools::DNSResolver::instance().get_dns_format_from_oa_address("donate.getmonero.org");
  EXPECT_STREQ("donate.getmonero.org", addr.c_str());
}

TEST(DNSResolver, Localhost)
{
  tools::DNSResolver resolver = tools::DNSResolver::create();

  bool avail, valid;
  std::vector<std::string> ips = resolver.get_ipv4("localhost", avail, valid);

  ASSERT_EQ(1, ips.size());
  ASSERT_EQ("127.0.0.1", ips[0]);
}

bool is_equal(const char *s, const std::vector<std::string> &v) { return v.size() == 1 && v[0] == s; }

TEST(DNS_PUBLIC, empty) { EXPECT_TRUE(tools::dns_utils::parse_dns_public("").empty()); }
TEST(DNS_PUBLIC, default) { EXPECT_TRUE(tools::dns_utils::parse_dns_public("tcp").size() > 0); }
TEST(DNS_PUBLIC, invalid_scheme) { EXPECT_TRUE(tools::dns_utils::parse_dns_public("invalid").empty()); }
TEST(DNS_PUBLIC, invalid_ip_alpha) { EXPECT_TRUE(tools::dns_utils::parse_dns_public("tcp://invalid").empty()); }
TEST(DNS_PUBLIC, invalid_ip_num1) { EXPECT_TRUE(tools::dns_utils::parse_dns_public("tcp://3").empty()); }
TEST(DNS_PUBLIC, invalid_ip_num3) { EXPECT_TRUE(tools::dns_utils::parse_dns_public("tcp://3.4.5").empty()); }
TEST(DNS_PUBLIC, invalid_ip_num4_extra) { EXPECT_TRUE(tools::dns_utils::parse_dns_public("tcp://3.4.5.6x").empty()); }
TEST(DNS_PUBLIC, invalid_ip_num4_range) { EXPECT_TRUE(tools::dns_utils::parse_dns_public("tcp://3.4.542.6").empty()); }
TEST(DNS_PUBLIC, invalid_ip_dot) { EXPECT_TRUE(tools::dns_utils::parse_dns_public("tcp://3.4.5.6.").empty()); }
TEST(DNS_PUBLIC, invalid_ip_num5) { EXPECT_TRUE(tools::dns_utils::parse_dns_public("tcp://3.4.5.6.7").empty()); }
TEST(DNS_PUBLIC, invalid_ip_4_missing) { EXPECT_TRUE(tools::dns_utils::parse_dns_public("tcp://3.4..7").empty()); }
TEST(DNS_PUBLIC, valid_ip_lo) { EXPECT_TRUE(is_equal("127.0.0.1", tools::dns_utils::parse_dns_public("tcp://127.0.0.1"))); }
TEST(DNS_PUBLIC, valid_ip) { EXPECT_TRUE(is_equal("3.4.5.6", tools::dns_utils::parse_dns_public("tcp://3.4.5.6"))); }
