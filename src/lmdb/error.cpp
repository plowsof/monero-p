// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

#include "error.h"

#include <lmdb.h>
#include <string>

namespace {
    struct category final : std::error_category
    {
        virtual const char* name() const noexcept override final
        {
            return "lmdb::error_category()";
        }

        virtual std::string message(int value) const override final
        {
            char const* const msg = mdb_strerror(value);
            if (msg)
                return msg;
            return "Unknown lmdb::error_category() value";
        }

        virtual std::error_condition default_error_condition(int value) const noexcept override final
        {
            switch (value)
            {
                case MDB_KEYEXIST:
                case MDB_NOTFOUND:
                    break; // map to nothing generic
                case MDB_PAGE_NOTFOUND:
                case MDB_CORRUPTED:
                    return std::errc::bad_address;
                case MDB_PANIC:
                case MDB_VERSION_MISMATCH:
                case MDB_INVALID:
                    break; // map to nothing generic
                case MDB_MAP_FULL:
                    return std::errc::no_buffer_space;
                case MDB_DBS_FULL:
                    break; // map to nothing generic
                case MDB_READERS_FULL:
                case MDB_TLS_FULL:
                    return std::errc::no_lock_available;
                case MDB_TXN_FULL:
                case MDB_CURSOR_FULL:
                case MDB_PAGE_FULL:
                case MDB_MAP_RESIZED:
                    break; // map to nothing generic
                case MDB_INCOMPATIBLE:
                    return std::errc::invalid_argument;
                case MDB_BAD_RSLOT:
                case MDB_BAD_TXN:
                case MDB_BAD_VALSIZE:
                case MDB_BAD_DBI:
                    return std::errc::invalid_argument;
                default:
                    return std::error_condition{value, std::generic_category()};
            }
            return std::error_condition{value, *this};
        }
    };
}

namespace lmdb
{
    std::error_category const& error_category() noexcept
    {
        static const category instance{};
        return instance;
    }
}

