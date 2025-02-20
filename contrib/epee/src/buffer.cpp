// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

// 

#include <limits>
#include <string.h>
#include "net/buffer.h"

#undef MONERO_DEFAULT_LOG_CATEGORY
#define MONERO_DEFAULT_LOG_CATEGORY "net.buffer"

namespace epee
{
namespace net_utils
{

void buffer::append(const void *data, size_t sz)
{
  const size_t capacity = storage.capacity();
  const size_t avail = capacity - storage.size();

  CHECK_AND_ASSERT_THROW_MES(storage.size() < std::numeric_limits<size_t>::max() - sz, "Too much data to append");

  // decide when to move
  if (sz > avail)
  {
    // we have to reallocate or move
    const bool move = size() + sz <= capacity;
    if (move)
    {
      const size_t bytes = storage.size() - offset;
      NET_BUFFER_LOG("appending " << sz << " from " << size() << " by moving " << bytes << " from offset " << offset << " first (forced)");
      memmove(storage.data(), storage.data() + offset, bytes);
      storage.resize(bytes);
      offset = 0;
    }
    else
    {
      NET_BUFFER_LOG("appending " << sz << " from " << size() << " by reallocating");
      std::vector<uint8_t> new_storage;
      size_t reserve = (((size() + sz) * 3 / 2) + 4095) & ~4095;
      new_storage.reserve(reserve);
      new_storage.resize(size());
      if (size() > 0)
        memcpy(new_storage.data(), storage.data() + offset, storage.size() - offset);
      offset = 0;
      std::swap(storage, new_storage);
    }
  }
  else
  {
    // we have space already
    if (size() <= 4096 && offset > 4096 * 16 && offset >= capacity / 2)
    {
      // we have little to move, and we're far enough into the buffer that it's probably a win to move anyway
      const size_t pos = storage.size() - offset;
      NET_BUFFER_LOG("appending " << sz << " from " << size() << " by moving " << pos << " from offset " << offset << " first (unforced)");
      memmove(storage.data(), storage.data() + offset, storage.size() - offset);
      storage.resize(pos);
      offset = 0;
    }
    else
    {
      NET_BUFFER_LOG("appending " << sz << " from " << size() << " by writing to existing capacity");
    }
  }

  // add the new data
  storage.insert(storage.end(), (const uint8_t*)data, (const uint8_t*)data + sz);

  NET_BUFFER_LOG("storage now " << offset << "/" << storage.size() << "/" << storage.capacity());
}

}
}
