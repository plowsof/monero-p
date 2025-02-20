// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 
// 
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#pragma once 

#include <unordered_set>
#include <mutex>

namespace tools
{
  template<typename T, size_t MAX_SIZE>
  class data_cache
  {
  public:
    void add(const T& value)
    {
      std::lock_guard<std::mutex> lock(m);
      if (data.insert(value).second)
      {
        T& old_value = buf[counter++ % MAX_SIZE];
        data.erase(old_value);
        old_value = value;
      }
    }

    bool has(const T& value) const
    {
      std::lock_guard<std::mutex> lock(m);
      return (data.find(value) != data.end());
    }

  private:
    mutable std::mutex m;
    std::unordered_set<T> data;
    T buf[MAX_SIZE] = {};
    size_t counter = 0;
  };
}
