// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 
// 
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#pragma once

#include <vector>
#include <deque>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <set>
#include "serialization.h"

namespace serialization
{
    //! @brief Is this type a STL-like container?
    //! To add a new container to be serialized, partially specialize the template is_container like so:
    template <typename T>     struct is_container:                                 std::false_type {};
    template <typename... TA> struct is_container<std::deque<TA...>>:              std::true_type {};
    template <typename... TA> struct is_container<std::map<TA...>>:                std::true_type {};
    template <typename... TA> struct is_container<std::multimap<TA...>>:           std::true_type {};
    template <typename... TA> struct is_container<std::set<TA...>>:                std::true_type {};
    template <typename... TA> struct is_container<std::unordered_map<TA...>>:      std::true_type {};
    template <typename... TA> struct is_container<std::unordered_multimap<TA...>>: std::true_type {};
    template <typename... TA> struct is_container<std::unordered_set<TA...>>:      std::true_type {};
    template <typename... TA> struct is_container<std::vector<TA...>>:             std::true_type {};
}

#include "container.h"

template <class Archive, class Container>
std::enable_if_t<::serialization::is_container<Container>::value, bool>
do_serialize(Archive &ar, Container &c)
{
    return ::do_serialize_container(ar, c);
}
