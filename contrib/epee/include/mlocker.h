// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

// 

#pragma once 

#include <map>
#include <boost/thread/mutex.hpp>

namespace epee
{
  class mlocker
  {
  public:
    mlocker(void *ptr, size_t len);
    ~mlocker();

    static size_t get_page_size();
    static size_t get_num_locked_pages();
    static size_t get_num_locked_objects();

    static void lock(void *ptr, size_t len);
    static void unlock(void *ptr, size_t len);

  private:
    static size_t page_size;
    static size_t num_locked_objects;

    static boost::mutex &mutex();
    static std::map<size_t, unsigned int> &map();
    static void lock_page(size_t page);
    static void unlock_page(size_t page);

    void *ptr;
    size_t len;
  };

  /// Locks memory while in scope
  ///
  /// Primarily useful for making sure that private keys don't get swapped out
  //  to disk
  template <class T>
  struct mlocked : public T {
    using type = T;

    mlocked(): T() { mlocker::lock(this, sizeof(T)); }
    mlocked(const T &t): T(t) { mlocker::lock(this, sizeof(T)); }
    mlocked(const mlocked<T> &mt): T(mt) { mlocker::lock(this, sizeof(T)); }
    mlocked(const T &&t): T(t) { mlocker::lock(this, sizeof(T)); }
    mlocked(const mlocked<T> &&mt): T(mt) { mlocker::lock(this, sizeof(T)); }
    mlocked<T> &operator=(const mlocked<T> &mt) { T::operator=(mt); return *this; }
    ~mlocked() { try { mlocker::unlock(this, sizeof(T)); } catch (...) { /* do not propagate */ } }
  };

  template<typename T>
  T& unwrap(mlocked<T>& src) { return src; }

  template<typename T>
  const T& unwrap(mlocked<T> const& src) { return src; }

  template <class T, size_t N>
  using mlocked_arr = mlocked<std::array<T, N>>;
}
