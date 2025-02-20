// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

// 

#include <atomic>
#include "gtest/gtest.h"
#include "misc_language.h"
#include "common/threadpool.h"

TEST(threadpool, wait_nothing)
{
  std::shared_ptr<tools::threadpool> tpool(tools::threadpool::getNewForUnitTests());
  tools::threadpool::waiter waiter(*tpool);;
  waiter.wait();
}

TEST(threadpool, wait_waits)
{
  std::shared_ptr<tools::threadpool> tpool(tools::threadpool::getNewForUnitTests());
  tools::threadpool::waiter waiter(*tpool);
  std::atomic<bool> b(false);
  tpool->submit(&waiter, [&b](){ epee::misc_utils::sleep_no_w(1000); b = true; });
  ASSERT_FALSE(b);
  waiter.wait();
  ASSERT_TRUE(b);
}

TEST(threadpool, one_thread)
{
  std::shared_ptr<tools::threadpool> tpool(tools::threadpool::getNewForUnitTests(1));
  tools::threadpool::waiter waiter(*tpool);

  std::atomic<unsigned int> counter(0);
  for (size_t n = 0; n < 4096; ++n)
  {
    tpool->submit(&waiter, [&counter](){++counter;});
  }
  waiter.wait();
  ASSERT_EQ(counter, 4096);
}

TEST(threadpool, many_threads)
{
  std::shared_ptr<tools::threadpool> tpool(tools::threadpool::getNewForUnitTests(256));
  tools::threadpool::waiter waiter(*tpool);

  std::atomic<unsigned int> counter(0);
  for (size_t n = 0; n < 4096; ++n)
  {
    tpool->submit(&waiter, [&counter](){++counter;});
  }
  waiter.wait();
  ASSERT_EQ(counter, 4096);
}

static uint64_t fibonacci(std::shared_ptr<tools::threadpool> tpool, uint64_t n)
{
  if (n <= 1)
    return n;
  uint64_t f1, f2;
  tools::threadpool::waiter waiter(*tpool);
  tpool->submit(&waiter, [&tpool, &f1, n](){ f1 = fibonacci(tpool, n-1); });
  tpool->submit(&waiter, [&tpool, &f2, n](){ f2 = fibonacci(tpool, n-2); });
  waiter.wait();
  return f1 + f2;
}

TEST(threadpool, reentrency)
{
  std::shared_ptr<tools::threadpool> tpool(tools::threadpool::getNewForUnitTests(4));
  tools::threadpool::waiter waiter(*tpool);

  uint64_t f = fibonacci(tpool, 13);
  waiter.wait();
  ASSERT_EQ(f, 233);
}

TEST(threadpool, reentrancy)
{
  std::shared_ptr<tools::threadpool> tpool(tools::threadpool::getNewForUnitTests(4));
  tools::threadpool::waiter waiter(*tpool);

  uint64_t f = fibonacci(tpool, 13);
  waiter.wait();
  ASSERT_EQ(f, 233);
}

TEST(threadpool, leaf_throws)
{
  std::shared_ptr<tools::threadpool> tpool(tools::threadpool::getNewForUnitTests());
  tools::threadpool::waiter waiter(*tpool);

  bool thrown = false, executed = false;
  tpool->submit(&waiter, [&](){
    try { tpool->submit(&waiter, [&](){ executed = true; }); }
    catch(const std::exception &e) { thrown = true; }
  }, true);
  waiter.wait();
  ASSERT_TRUE(thrown);
  ASSERT_FALSE(executed);
}

TEST(threadpool, leaf_reentrancy)
{
  std::shared_ptr<tools::threadpool> tpool(tools::threadpool::getNewForUnitTests(4));
  tools::threadpool::waiter waiter(*tpool);

  std::atomic<int> counter(0);
  for (int i = 0; i < 1000; ++i)
  {
    tpool->submit(&waiter, [&](){
      tools::threadpool::waiter waiter(*tpool);
      for (int j = 0; j < 500; ++j)
      {
        tpool->submit(&waiter, [&](){ ++counter; }, true);
      }
      waiter.wait();
    });
  }
  waiter.wait();
  ASSERT_EQ(counter, 500000);
}
