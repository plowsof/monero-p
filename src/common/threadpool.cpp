// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
//
#include "misc_log_ex.h"
#include "common/threadpool.h"

#include "cryptonote_config.h"
#include "common/util.h"

static __thread int depth = 0;
static __thread bool is_leaf = false;

namespace tools
{
threadpool::threadpool(unsigned int max_threads) : running(true), active(0) {
  create(max_threads);
}

threadpool::~threadpool() {
  destroy();
}

void threadpool::destroy() {
  try
  {
    const boost::unique_lock<boost::mutex> lock(mutex);
    running = false;
    has_work.notify_all();
  }
  catch (...)
  {
    // if the lock throws, we're just do it without a lock and hope,
    // since the alternative is terminate
    running = false;
    has_work.notify_all();
  }
  for (size_t i = 0; i<threads.size(); i++) {
    try { threads[i].join(); }
    catch (...) { /* ignore */ }
  }
  threads.clear();
}

void threadpool::recycle() {
  destroy();
  create(max);
}

void threadpool::create(unsigned int max_threads) {
  const boost::unique_lock<boost::mutex> lock(mutex);
  boost::thread::attributes attrs;
  attrs.set_stack_size(THREAD_STACK_SIZE);
  max = max_threads ? max_threads : tools::get_max_concurrency();
  size_t i = max ? max - 1 : 0;
  running = true;
  while(i--) {
    threads.push_back(boost::thread(attrs, boost::bind(&threadpool::run, this, false)));
  }
}

void threadpool::submit(waiter *obj, std::function<void()> f, bool leaf) {
  CHECK_AND_ASSERT_THROW_MES(!is_leaf, "A leaf routine is using a thread pool");
  boost::unique_lock<boost::mutex> lock(mutex);
  if (!leaf && ((active == max && !queue.empty()) || depth > 0)) {
    // if all available threads are already running
    // and there's work waiting, just run in current thread
    lock.unlock();
    ++depth;
    is_leaf = leaf;
    f();
    --depth;
    is_leaf = false;
  } else {
    if (obj)
      obj->inc();
    if (leaf)
      queue.push_front({obj, f, leaf});
    else
      queue.push_back({obj, f, leaf});
    has_work.notify_one();
  }
}

unsigned int threadpool::get_max_concurrency() const {
  return max;
}

threadpool::waiter::~waiter()
{
  try
  {
    boost::unique_lock<boost::mutex> lock(mt);
    if (num)
      MERROR("wait should have been called before waiter dtor - waiting now");
  }
  catch (...) { /* ignore */ }
  try
  {
    wait();
  }
  catch (const std::exception &e)
  {
    /* ignored */
  }
}

bool threadpool::waiter::wait() {
  pool.run(true);
  boost::unique_lock<boost::mutex> lock(mt);
  while(num)
    cv.wait(lock);
  return !error();
}

void threadpool::waiter::inc() {
  const boost::unique_lock<boost::mutex> lock(mt);
  num++;
}

void threadpool::waiter::dec() {
  const boost::unique_lock<boost::mutex> lock(mt);
  num--;
  if (!num)
    cv.notify_all();
}

void threadpool::run(bool flush) {
  boost::unique_lock<boost::mutex> lock(mutex);
  while (running) {
    entry e;
    while(queue.empty() && running)
    {
      if (flush)
        return;
      has_work.wait(lock);
    }
    if (!running) break;

    active++;
    e = std::move(queue.front());
    queue.pop_front();
    lock.unlock();
    ++depth;
    is_leaf = e.leaf;
    try { e.f(); }
    catch (const std::exception &ex) { e.wo->set_error(); try { MERROR("Exception in threadpool job: " << ex.what()); } catch (...) {} }
    --depth;
    is_leaf = false;

    if (e.wo)
      e.wo->dec();
    lock.lock();
    active--;
  }
}
}
