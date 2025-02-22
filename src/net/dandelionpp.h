// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
//

#pragma once

#include <boost/uuid/uuid.hpp>
#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

#include "span.h"

namespace net
{
namespace dandelionpp
{
    //! Assists with mapping source -> stem and tracking connections for stem.
    class connection_map
    {
        // Make sure to update clone method if changing members
        std::vector<boost::uuids::uuid> out_mapping_; //<! Current outgoing uuid connection at index.
        std::vector<std::pair<boost::uuids::uuid, std::size_t>> in_mapping_; //<! uuid source to an `out_mapping_` index.
        std::vector<std::size_t> usage_count_;

        // Use clone method to prevent "hidden" copies.
        connection_map(const connection_map&) = default;

    public:
        using value_type = boost::uuids::uuid;
        using size_type = std::vector<boost::uuids::uuid>::size_type;
        using difference_type = std::vector<boost::uuids::uuid>::difference_type;
        using reference = const boost::uuids::uuid&;
        using const_reference = reference;
        using iterator = std::vector<boost::uuids::uuid>::const_iterator;
        using const_iterator = iterator;

        //! Initialized with zero stem connections.
        explicit connection_map()
          : connection_map(std::vector<boost::uuids::uuid>{}, 0)
        {}

        //! Initialized with `out_connections` and `stem_count`.
        explicit connection_map(std::vector<boost::uuids::uuid> out_connections, std::size_t stems);

        connection_map(connection_map&&) = default;
        ~connection_map() noexcept;
        connection_map& operator=(connection_map&&) = default;
        connection_map& operator=(const connection_map&) = delete;

        //! \return An exact duplicate of `this` map.
        connection_map clone() const;

        //! \return First stem connection.
        const_iterator begin() const noexcept
        {
            return out_mapping_.begin();
        }

        //! \return One-past the last stem connection.
        const_iterator end() const noexcept
        {
            return out_mapping_.end();
        }

        /*! Merges in current connections with the previous set of connections.
            If a connection died, a new one will take its place in the stem or
            the stem is marked as dead.

            \param connections Current outbound connection ids.
            \return True if any updates to `get_connections()` was made. */
        bool update(std::vector<boost::uuids::uuid> current);

        //! \return Number of outgoing connections in use.
        std::size_t size() const noexcept;

        //! \return Current stem mapping for `source` or `nil_uuid()` if none is possible.
        boost::uuids::uuid get_stem(const boost::uuids::uuid& source);
    };
} // dandelionpp
} // net
