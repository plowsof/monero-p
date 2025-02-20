// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <lmdb.h>
#include <memory>

#include "lmdb/error.h"

//! Uses C++ type system to differentiate between cursors
#define MONERO_CURSOR(name)                                    \
    struct close_ ## name : ::lmdb::close_cursor {};           \
    using name = std::unique_ptr< MDB_cursor, close_ ## name >;

namespace lmdb
{
    struct abort_txn
    {
        void operator()(MDB_txn* ptr) const noexcept
        {
            if (ptr)
                mdb_txn_abort(ptr);
        }
    };

    /*!
        Only valid if used via `create_read_txn()`. Decrements active count in
        associated `context`, and aborts a LMDB transaction (`mdb_txn_abort`).
    */
    struct release_read_txn
    { 
        void operator()(MDB_txn* ptr) const noexcept;
        // implementation in database.cpp
    };

    /*!
        Only valid if used via `create_write_txn()`. Decrements active count in
        associated `context`, and aborts a LMDB transaction (`mdb_txn_abort`).
    */
    struct abort_write_txn
    {
        void operator()(MDB_txn* ptr) const noexcept
        {
            release_read_txn{}(ptr);
        }
    };

    struct close_cursor
    {
        void operator()(MDB_cursor* ptr) const noexcept
        {
            if (ptr)
                mdb_cursor_close(ptr);
        }
    };

    template<typename D>
    inline expect<std::unique_ptr<MDB_cursor, D>>
    open_cursor(MDB_txn& txn, MDB_dbi tbl) noexcept
    {
        MDB_cursor* cur = nullptr;
        MONERO_LMDB_CHECK(mdb_cursor_open(&txn, tbl, &cur));
        return std::unique_ptr<MDB_cursor, D>{cur};
    }

    // The below use the C++ type system to designate `MDB_txn` status.

    using suspended_txn = std::unique_ptr<MDB_txn, abort_txn>;
    using read_txn = std::unique_ptr<MDB_txn, release_read_txn>;
    using write_txn = std::unique_ptr<MDB_txn, abort_write_txn>;
} // lmdb
