// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 
// 
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#pragma once

#ifdef __cplusplus
#include <array>
#include <cstddef>

extern "C" {
#endif

void *memwipe(void *src, size_t n);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
namespace tools {

  /// Scrubs data in the contained type upon destruction.
  ///
  /// Primarily useful for making sure that private keys don't stick around in
  /// memory after the objects that held them have gone out of scope.
  template <class T>
  struct scrubbed : public T {
    using type = T;

    ~scrubbed() {
      scrub();
    }

    /// Destroy the contents of the contained type.
    void scrub() {
      static_assert(std::is_pod<T>::value,
                    "T cannot be auto-scrubbed. T must be POD.");
      static_assert(std::is_trivially_destructible<T>::value,
                    "T cannot be auto-scrubbed. T must be trivially destructable.");
      memwipe(this, sizeof(T));
    }
  };

  template<typename T>
  T& unwrap(scrubbed<T>& src) { return src; }

  template<typename T>
  const T& unwrap(scrubbed<T> const& src) { return src; }

  template <class T, size_t N>
  using scrubbed_arr = scrubbed<std::array<T, N>>;
} // namespace tools

#endif // __cplusplus
