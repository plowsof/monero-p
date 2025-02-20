// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 

#pragma once

#include <boost/optional/optional.hpp>
#include <stddef.h>
#include <vector>
#include <string>
#include "memwipe.h"
#include "fnv1.h"

namespace epee
{
  class wipeable_string
  {
  public:
    typedef char value_type;

    wipeable_string() {}
    wipeable_string(const wipeable_string &other);
    wipeable_string(wipeable_string &&other);
    wipeable_string(const std::string &other);
    wipeable_string(std::string &&other);
    wipeable_string(const char *s);
    wipeable_string(const char *s, size_t len);
    ~wipeable_string();
    void wipe();
    void push_back(char c);
    void operator+=(char c);
    void operator+=(const std::string &s);
    void operator+=(const epee::wipeable_string &s);
    void operator+=(const char *s);
    void append(const char *ptr, size_t len);
    char pop_back();
    const char *data() const noexcept { return buffer.data(); }
    char *data() noexcept { return buffer.data(); }
    size_t size() const noexcept { return buffer.size(); }
    size_t length() const noexcept { return buffer.size(); }
    bool empty() const noexcept { return buffer.empty(); }
    void trim();
    void split(std::vector<wipeable_string> &fields) const;
    boost::optional<wipeable_string> parse_hexstr() const;
    template<typename T> inline bool hex_to_pod(T &pod) const;
    template<typename T> inline bool hex_to_pod(tools::scrubbed<T> &pod) const { return hex_to_pod(unwrap(pod)); }
    void resize(size_t sz);
    void reserve(size_t sz);
    void clear();
    bool operator==(const wipeable_string &other) const noexcept { return buffer == other.buffer; }
    bool operator!=(const wipeable_string &other) const noexcept { return buffer != other.buffer; }
    wipeable_string &operator=(wipeable_string &&other);
    wipeable_string &operator=(const wipeable_string &other);

  private:
    void grow(size_t sz, size_t reserved = 0);

  private:
    std::vector<char> buffer;
  };

  template<typename T> inline bool wipeable_string::hex_to_pod(T &pod) const
  {
    static_assert(std::is_pod<T>::value, "expected pod type");
    if (size() != sizeof(T) * 2)
      return false;
    boost::optional<epee::wipeable_string> blob = parse_hexstr();
    if (!blob)
      return false;
    if (blob->size() != sizeof(T))
      return false;
    pod = *(const T*)blob->data();
    return true;
  }
}

namespace std
{
  template<> struct hash<epee::wipeable_string>
  {
    size_t operator()(const epee::wipeable_string &s) const
    {
      return epee::fnv::FNV1a(s.data(), s.size());
    }
  };
}
