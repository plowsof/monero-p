// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
//
#pragma once

#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <cstddef>
#include <deque>
#include <functional>
#include <utility>
#include <vector>
#include <stdexcept>

namespace tools
{
//! A global thread pool
class threadpool
{
public:
  static threadpool& getInstanceForCompute() {
    static threadpool instance;
    return instance;
  }
  static threadpool& getInstanceForIO() {
    static threadpool instance(8);
    return instance;
  }
  static threadpool *getNewForUnitTests(unsigned max_threads = 0) {
    return new threadpool(max_threads);
  }

  // The waiter lets the caller know when all of its
  // tasks are completed.
  class waiter {
    boost::mutex mt;
    boost::condition_variable cv;
    threadpool &pool;
    int num;
    bool error_flag;
    public:
    void inc();
    void dec();
    bool wait();  //! Wait for a set of tasks to finish, returns false iff any error
    void set_error() noexcept { error_flag = true; }
    bool error() const noexcept { return error_flag; }
    waiter(threadpool &pool) : pool(pool), num(0), error_flag(false) {}
    ~waiter();
  };

  // Submit a task to the pool. The waiter pointer may be
  // NULL if the caller doesn't care to wait for the
  // task to finish.
  void submit(waiter *waiter, std::function<void()> f, bool leaf = false);

  // destroy and recreate threads
  void recycle();

  unsigned int get_max_concurrency() const;

  ~threadpool();

  private:
    threadpool(unsigned int max_threads = 0);
    void destroy();
    void create(unsigned int max_threads);
    typedef struct entry {
      waiter *wo;
      std::function<void()> f;
      bool leaf;
    } entry;
    std::deque<entry> queue;
    boost::condition_variable has_work;
    boost::mutex mutex;
    std::vector<boost::thread> threads;
    unsigned int active;
    unsigned int max;
    bool running;
    void run(bool flush = false);
};

}
