// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

//

#include "byte_stream.h"

#include <algorithm>
#include <limits>
#include <utility>

#include <iostream>

namespace
{
  constexpr const std::size_t minimum_increase = 4096;
}

namespace epee
{
  void byte_stream::overflow(const std::size_t requested)
  {
    // Recalculating `need` bytes removes at least one instruction from every
    // inlined `put` call in header

    assert(available() < requested);
    const std::size_t need = requested - available();

    const std::size_t len = size();
    const std::size_t cap = capacity();
    const std::size_t increase = std::max(std::max(need, cap), minimum_increase);

    next_write_ = nullptr;
    end_ = nullptr;

    buffer_ = byte_buffer_increase(std::move(buffer_), cap, increase);
    if (!buffer_)
      throw std::bad_alloc{};

    next_write_ = buffer_.get() + len;
    end_ = buffer_.get() + cap + increase;
  }

  byte_stream::byte_stream(byte_stream&& rhs) noexcept
    : buffer_(std::move(rhs.buffer_)),
      next_write_(rhs.next_write_),
      end_(rhs.end_)
  {
    rhs.next_write_ = nullptr;
    rhs.end_ = nullptr;
  }

  byte_stream& byte_stream::operator=(byte_stream&& rhs) noexcept
  {
    if (this != std::addressof(rhs))
    {
      buffer_ = std::move(rhs.buffer_);
      next_write_ = rhs.next_write_;
      end_ = rhs.end_;
      rhs.next_write_ = nullptr;
      rhs.end_ = nullptr;
    }
    return *this;
  }

  byte_buffer byte_stream::take_buffer() noexcept
  {
    byte_buffer out{std::move(buffer_)};
    next_write_ = nullptr;
    end_ = nullptr;
    return out;
  }
}
