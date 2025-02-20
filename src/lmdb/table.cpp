// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

#include "table.h"

namespace lmdb
{
    expect<MDB_dbi> table::open(MDB_txn& write_txn) const noexcept
    {
        MONERO_PRECOND(name != nullptr);

        MDB_dbi out;
        MONERO_LMDB_CHECK(mdb_dbi_open(&write_txn, name, flags, &out));
        if (key_cmp && !(flags & MDB_INTEGERKEY))
            MONERO_LMDB_CHECK(mdb_set_compare(&write_txn, out, key_cmp));
        if (value_cmp && !(flags & MDB_INTEGERDUP))
            MONERO_LMDB_CHECK(mdb_set_dupsort(&write_txn, out, value_cmp));
        return out;
    }
}
