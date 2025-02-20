// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <atomic>
#include <cstddef>
#include <lmdb.h>
#include <memory>
#include <type_traits>

#include "common/expect.h"
#include "lmdb/error.h"
#include "lmdb/transaction.h"

namespace lmdb
{
    //! Closes LMDB environment handle.
    struct close_env
    {
        void operator()(MDB_env* ptr) const noexcept
        {
            if (ptr)
                mdb_env_close(ptr);
        }
    };

    using environment = std::unique_ptr<MDB_env, close_env>;

    //! \return LMDB environment at `path` with a max of `max_dbs` tables.
    expect<environment> open_environment(const char* path, MDB_dbi max_dbs) noexcept;

    //! Context given to LMDB.
    struct context
    {
        std::atomic<std::size_t> active;
        std::atomic_flag lock;
    };

    //! Manages a LMDB environment for safe memory-map resizing. Thread-safe.
    class database
    {
        environment env;
        context ctx;

        //! \return The LMDB environment associated with the object.
        MDB_env* handle() const noexcept { return env.get(); }

        expect<write_txn> do_create_txn(unsigned int flags) noexcept;

    public: 
        database(environment env);

        database(database&&) = delete;
        database(database const&) = delete;

        virtual ~database() noexcept;

        database& operator=(database&&) = delete;
        database& operator=(database const&) = delete;

        /*!
            Resize the memory map for the LMDB environment. Will block until
            all reads/writes on the environment complete.
        */
        expect<void> resize() noexcept;

        //! \return A read only LMDB transaction, reusing `txn` if provided.
        expect<read_txn> create_read_txn(suspended_txn txn = nullptr) noexcept;

        //! \return `txn` after releasing context.
        expect<suspended_txn> reset_txn(read_txn txn) noexcept;

        //! \return A read-write LMDB transaction.
        expect<write_txn> create_write_txn() noexcept;

        //! Commit the read-write transaction.
        expect<void> commit(write_txn txn) noexcept;

        /*!
            Create a write transaction, pass it to `f`, then try to commit
            the write if `f` succeeds.

            \tparam F must be callable with signature `expect<T>(MDB_txn&)`.
            \param f must be re-startable if `lmdb::error(MDB_MAP_FULL)`.

            \return The result of calling `f`.
        */
        template<typename F>
        auto try_write(F f, unsigned attempts = 3) -> decltype(f(std::declval<MDB_txn&>()))
        {
            for (unsigned i = 0; i < attempts; ++i)
            {
                expect<write_txn> txn = create_write_txn();
                if (!txn)
                    return txn.error();

                MONERO_PRECOND(*txn != nullptr);
                const auto wrote = f(*(*txn));
                if (wrote)
                {
                    const auto committed = commit(std::move(*txn));
                    if (committed)
                      return wrote;
                    if (committed != lmdb::error(MDB_MAP_FULL))
                      return committed.error();
                }
                else if (wrote != lmdb::error(MDB_MAP_FULL))
                    return wrote;

                txn->reset();
                MONERO_CHECK(this->resize());
            }
            return {lmdb::error(MDB_MAP_FULL)};
        }
    };
} // lmdb

