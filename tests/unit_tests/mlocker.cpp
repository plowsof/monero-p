// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

// 

#include "gtest/gtest.h"

#include "misc_log_ex.h"
#include "mlocker.h"

#if defined __GNUC__ && !defined _WIN32
#define HAVE_MLOCK 1
#endif

#ifdef HAVE_MLOCK

#define BASE(data) (char*)(((uintptr_t)(data.get() + page_size - 1)) / page_size * page_size)

TEST(mlocker, distinct_1)
{
  const size_t page_size = epee::mlocker::get_page_size();
  ASSERT_TRUE(page_size > 0);
  const size_t base_pages = epee::mlocker::get_num_locked_pages();
  const size_t base_objects = epee::mlocker::get_num_locked_objects();
  std::unique_ptr<char[]> data{new char[8 * page_size]};
  std::shared_ptr<epee::mlocker> m0{new epee::mlocker(BASE(data), 1)};
  std::shared_ptr<epee::mlocker> m1{new epee::mlocker(BASE(data) + 2 * page_size, 1)};
  std::shared_ptr<epee::mlocker> m2{new epee::mlocker(BASE(data) + 3 * page_size, 1)};
  ASSERT_EQ(epee::mlocker::get_num_locked_pages(), base_pages + 3);
  ASSERT_EQ(epee::mlocker::get_num_locked_objects(), base_objects + 3);
  m0 = NULL;
  m1 = NULL;
  m2 = NULL;
  ASSERT_EQ(epee::mlocker::get_num_locked_pages(), base_pages + 0);
  ASSERT_EQ(epee::mlocker::get_num_locked_objects(), base_objects + 0);
}

TEST(mlocker, distinct_full_page)
{
  const size_t page_size = epee::mlocker::get_page_size();
  ASSERT_TRUE(page_size > 0);
  const size_t base_pages = epee::mlocker::get_num_locked_pages();
  const size_t base_objects = epee::mlocker::get_num_locked_objects();
  std::unique_ptr<char[]> data{new char[8 * page_size]};
  std::shared_ptr<epee::mlocker> m0{new epee::mlocker(BASE(data), page_size)};
  std::shared_ptr<epee::mlocker> m1{new epee::mlocker(BASE(data) + 2 * page_size, page_size)};
  std::shared_ptr<epee::mlocker> m2{new epee::mlocker(BASE(data) + 3 * page_size, page_size)};
  ASSERT_EQ(epee::mlocker::get_num_locked_pages(), base_pages + 3);
  ASSERT_EQ(epee::mlocker::get_num_locked_objects(), base_objects + 3);
  m0 = NULL;
  m1 = NULL;
  m2 = NULL;
  ASSERT_EQ(epee::mlocker::get_num_locked_pages(), base_pages + 0);
  ASSERT_EQ(epee::mlocker::get_num_locked_objects(), base_objects + 0);
}

TEST(mlocker, identical)
{
  const size_t page_size = epee::mlocker::get_page_size();
  ASSERT_TRUE(page_size >= 32);
  const size_t base_pages = epee::mlocker::get_num_locked_pages();
  const size_t base_objects = epee::mlocker::get_num_locked_objects();
  std::unique_ptr<char[]> data{new char[8 * page_size]};
  std::shared_ptr<epee::mlocker> m0{new epee::mlocker(BASE(data) + page_size, 32)};
  std::shared_ptr<epee::mlocker> m1{new epee::mlocker(BASE(data) + page_size, 32)};
  std::shared_ptr<epee::mlocker> m2{new epee::mlocker(BASE(data) + page_size, 32)};
  ASSERT_EQ(epee::mlocker::get_num_locked_pages(), base_pages + 1);
  ASSERT_EQ(epee::mlocker::get_num_locked_objects(), base_objects + 3);
  m1 = NULL;
  ASSERT_EQ(epee::mlocker::get_num_locked_pages(), base_pages + 1);
  ASSERT_EQ(epee::mlocker::get_num_locked_objects(), base_objects + 2);
  m0 = NULL;
  m2 = NULL;
  ASSERT_EQ(epee::mlocker::get_num_locked_pages(), base_pages + 0);
  ASSERT_EQ(epee::mlocker::get_num_locked_objects(), base_objects + 0);
}

TEST(mlocker, overlapping_small)
{
  const size_t page_size = epee::mlocker::get_page_size();
  ASSERT_TRUE(page_size >= 64);
  const size_t base_pages = epee::mlocker::get_num_locked_pages();
  const size_t base_objects = epee::mlocker::get_num_locked_objects();
  std::unique_ptr<char[]> data{new char[8 * page_size]};
  std::shared_ptr<epee::mlocker> m0{new epee::mlocker(BASE(data), 32)};
  std::shared_ptr<epee::mlocker> m1{new epee::mlocker(BASE(data) + 16, 32)};
  std::shared_ptr<epee::mlocker> m2{new epee::mlocker(BASE(data) + 8, 32)};
  ASSERT_EQ(epee::mlocker::get_num_locked_pages(), base_pages + 1);
  ASSERT_EQ(epee::mlocker::get_num_locked_objects(), base_objects + 3);
  m1 = NULL;
  ASSERT_EQ(epee::mlocker::get_num_locked_pages(), base_pages + 1);
  ASSERT_EQ(epee::mlocker::get_num_locked_objects(), base_objects + 2);
  m2 = NULL;
  m0 = NULL;
  ASSERT_EQ(epee::mlocker::get_num_locked_pages(), base_pages + 0);
  ASSERT_EQ(epee::mlocker::get_num_locked_objects(), base_objects + 0);
}

TEST(mlocker, multi_page)
{
  const size_t page_size = epee::mlocker::get_page_size();
  ASSERT_TRUE(page_size > 0);
  const size_t base_pages = epee::mlocker::get_num_locked_pages();
  const size_t base_objects = epee::mlocker::get_num_locked_objects();
  std::unique_ptr<char[]> data{new char[8 * page_size]};
  std::shared_ptr<epee::mlocker> m0{new epee::mlocker(BASE(data) + page_size, page_size * 3)};
  ASSERT_EQ(epee::mlocker::get_num_locked_pages(), base_pages + 3);
  ASSERT_EQ(epee::mlocker::get_num_locked_objects(), base_objects + 1);
  std::shared_ptr<epee::mlocker> m1{new epee::mlocker(BASE(data) + page_size * 7, page_size)};
  ASSERT_EQ(epee::mlocker::get_num_locked_pages(), base_pages + 4);
  ASSERT_EQ(epee::mlocker::get_num_locked_objects(), base_objects + 2);
  m0 = NULL;
  ASSERT_EQ(epee::mlocker::get_num_locked_pages(), base_pages + 1);
  ASSERT_EQ(epee::mlocker::get_num_locked_objects(), base_objects + 1);
  m1 = NULL;
  ASSERT_EQ(epee::mlocker::get_num_locked_pages(), base_pages + 0);
  ASSERT_EQ(epee::mlocker::get_num_locked_objects(), base_objects + 0);
}

TEST(mlocker, cross_page)
{
  const size_t page_size = epee::mlocker::get_page_size();
  ASSERT_TRUE(page_size > 32);
  const size_t base_pages = epee::mlocker::get_num_locked_pages();
  const size_t base_objects = epee::mlocker::get_num_locked_objects();
  std::unique_ptr<char[]> data{new char[2 * page_size]};
  std::shared_ptr<epee::mlocker> m0{new epee::mlocker(BASE(data) + page_size - 1, 2)};
  ASSERT_EQ(epee::mlocker::get_num_locked_pages(), base_pages + 2);
  ASSERT_EQ(epee::mlocker::get_num_locked_objects(), base_objects + 1);
  m0 = NULL;
  ASSERT_EQ(epee::mlocker::get_num_locked_pages(), base_pages + 0);
  ASSERT_EQ(epee::mlocker::get_num_locked_objects(), base_objects + 0);
}

TEST(mlocker, redundant)
{
  const size_t page_size = epee::mlocker::get_page_size();
  ASSERT_TRUE(page_size > 0);
  const size_t base_pages = epee::mlocker::get_num_locked_pages();
  const size_t base_objects = epee::mlocker::get_num_locked_objects();
  std::unique_ptr<char[]> data{new char[2 * page_size]};
  ASSERT_EQ(epee::mlocker::get_num_locked_pages(), base_pages + 0);
  ASSERT_EQ(epee::mlocker::get_num_locked_objects(), base_objects + 0);
  std::shared_ptr<epee::mlocker> m0{new epee::mlocker(BASE(data), 32)};
  ASSERT_EQ(epee::mlocker::get_num_locked_pages(), base_pages + 1);
  ASSERT_EQ(epee::mlocker::get_num_locked_objects(), base_objects + 1);
  std::shared_ptr<epee::mlocker> m1{new epee::mlocker(BASE(data), 32)};
  ASSERT_EQ(epee::mlocker::get_num_locked_pages(), base_pages + 1);
  ASSERT_EQ(epee::mlocker::get_num_locked_objects(), base_objects + 2);
  m1 = NULL;
  ASSERT_EQ(epee::mlocker::get_num_locked_pages(), base_pages + 1);
  ASSERT_EQ(epee::mlocker::get_num_locked_objects(), base_objects + 1);
  m0 = NULL;
  ASSERT_EQ(epee::mlocker::get_num_locked_pages(), base_pages + 0);
  ASSERT_EQ(epee::mlocker::get_num_locked_objects(), base_objects + 0);
}

TEST(mlocker, mlocked)
{
  const size_t base_pages = epee::mlocker::get_num_locked_pages();
  const size_t base_objects = epee::mlocker::get_num_locked_objects();
  {
    struct Foo { uint64_t u; };
    epee::mlocked<Foo> l;
    ASSERT_EQ(epee::mlocker::get_num_locked_pages(), base_pages + 1);
    ASSERT_EQ(epee::mlocker::get_num_locked_objects(), base_objects + 1);
  }
  ASSERT_EQ(epee::mlocker::get_num_locked_pages(), base_pages + 0);
  ASSERT_EQ(epee::mlocker::get_num_locked_objects(), base_objects + 0);
}

#endif
