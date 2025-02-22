// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

// 

#include "gtest/gtest.h"

extern "C" {
#include "crypto/keccak.h"
}

#define KECCAK_BLOCKLEN 136

#define TEST_KECCAK(sz, chunks) \
  std::string data; \
  data.resize(sz); \
  for (size_t i = 0; i < data.size(); ++i) \
    data[i] = i * 17; \
  uint8_t md0[32], md1[32]; \
  keccak((const uint8_t*)data.data(), data.size(), md0, 32); \
  KECCAK_CTX ctx; \
  keccak_init(&ctx); \
  size_t offset = 0; \
  for (size_t i = 0; i < sizeof(chunks) / sizeof(chunks[0]); ++i) \
  { \
    ASSERT_TRUE(offset + chunks[i] <= data.size()); \
    keccak_update(&ctx, (const uint8_t*)data.data() + offset, chunks[i]); \
    offset += chunks[i]; \
  } \
  ASSERT_TRUE(offset == data.size()); \
  keccak_finish(&ctx, md1); \
  ASSERT_EQ(memcmp(md0, md1, 32), 0);

TEST(keccak, 0_and_0)
{
  static const size_t chunks[] = {0};
  TEST_KECCAK(0, chunks);
}

TEST(keccak, 1_and_1)
{
  static const size_t chunks[] = {1};
  TEST_KECCAK(1, chunks);
}

TEST(keccak, 1_and_0_1_0)
{
  static const size_t chunks[] = {0, 1, 0};
  TEST_KECCAK(1, chunks);
}

TEST(keccak, 2_and_1_1)
{
  static const size_t chunks[] = {1, 1};
  TEST_KECCAK(2, chunks);
}

TEST(keccak, 4_and_0_0_1_0_2_1_0)
{
  static const size_t chunks[] = {0, 0, 1, 0, 2, 1, 0};
  TEST_KECCAK(4, chunks);
}

TEST(keccak, 15_and_1_14)
{
  static const size_t chunks[] = {1, 14};
  TEST_KECCAK(15, chunks);
}

TEST(keccak, 135_and_134_1)
{
  static const size_t chunks[] = {134, 1};
  TEST_KECCAK(135, chunks);
}

TEST(keccak, 135_and_135_0)
{
  static const size_t chunks[] = {135, 0};
  TEST_KECCAK(135, chunks);
}

TEST(keccak, 135_and_0_135)
{
  static const size_t chunks[] = {0, 135};
  TEST_KECCAK(135, chunks);
}

TEST(keccak, 136_and_135_1)
{
  static const size_t chunks[] = {135, 1};
  TEST_KECCAK(136, chunks);
}

TEST(keccak, 136_and_136_0)
{
  static const size_t chunks[] = {136, 0};
  TEST_KECCAK(136, chunks);
}

TEST(keccak, 136_and_0_136)
{
  static const size_t chunks[] = {0, 136};
  TEST_KECCAK(136, chunks);
}

TEST(keccak, 136_and_136)
{
  static const size_t chunks[] = {136};
  TEST_KECCAK(136, chunks);
}

TEST(keccak, 137_and_136_1)
{
  static const size_t chunks[] = {136, 1};
  TEST_KECCAK(137, chunks);
}

TEST(keccak, 137_and_1_136)
{
  static const size_t chunks[] = {1, 136};
  TEST_KECCAK(137, chunks);
}

TEST(keccak, alignment)
{
  uint8_t data[6064];
  __attribute__ ((aligned(16))) char adata[6000];

  for (size_t i = 0; i < sizeof(data) / sizeof(data[0]); ++i)
    data[i] = i & 1;

  uint8_t md[32], amd[32];
  for (int offset = 0; offset < 64; ++offset)
  {
    memcpy(adata, data + offset, 6000);
    keccak((const uint8_t*)&data[offset], 6000, md, 32);
    keccak((const uint8_t*)adata, 6000, amd, 32);
    ASSERT_TRUE(!memcmp(md, amd, 32));
  }
}
