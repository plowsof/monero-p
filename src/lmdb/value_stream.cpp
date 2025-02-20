// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

#include "value_stream.h"

#include <stdexcept>

#include "common/expect.h"
#include "lmdb/error.h"
#include "lmdb/util.h"

namespace lmdb
{
    namespace stream
    {
        mdb_size_t count(MDB_cursor* cur)
        {
            mdb_size_t out = 0;
            if (cur)
            {
                const int rc = mdb_cursor_count(cur, &out);
                if (rc)
                    MONERO_THROW(lmdb::error(rc), "mdb_cursor_count");
            }
            return out;
        }

        std::pair<epee::span<const std::uint8_t>, epee::span<const std::uint8_t>>
        get(MDB_cursor& cur, MDB_cursor_op op, std::size_t key, std::size_t value)
        {
            MDB_val key_bytes{};
            MDB_val value_bytes{};
            const int rc = mdb_cursor_get(&cur, &key_bytes, &value_bytes, op);
            if (rc)
            {
                if (rc == MDB_NOTFOUND)
                    return {};
                MONERO_THROW(lmdb::error(rc), "mdb_cursor_get");
            }

            if (key && key != key_bytes.mv_size)
                MONERO_THROW(lmdb::error(MDB_BAD_VALSIZE), "mdb_cursor_get key");

            if (value && (value_bytes.mv_size % value != 0 || value_bytes.mv_size == 0))
                MONERO_THROW(lmdb::error(MDB_BAD_VALSIZE), "mdb_cursor_get value");

            return {lmdb::to_byte_span(key_bytes), lmdb::to_byte_span(value_bytes)};
        }
    }
}

