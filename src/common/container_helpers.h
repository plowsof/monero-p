// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
//

// Miscellaneous container helpers.

#pragma once

//local headers

//third party headers

//standard headers
#include <algorithm>
#include <unordered_map>
#include <utility>

//forward declarations


namespace tools
{

/// convert an arbitrary function to functor
template <typename F>
inline auto as_functor(F f)
{
    return [f = std::move(f)](auto&&... args) { return f(std::forward<decltype(args)>(args)...); };
}
/// convert a binary comparison function to a functor
/// note: for most use-cases 'const T&' will work because only non-trivial types need a user-defined comparison operation
template <typename T, typename ComparisonOpT = bool(const T&, const T&)>
inline auto compare_func(ComparisonOpT comparison_op_func)
{
    static_assert(
            std::is_same<
                    bool,
                    decltype(
                        comparison_op_func(
                            std::declval<std::remove_cv_t<T>>(),
                            std::declval<std::remove_cv_t<T>>()
                        )
                    )
                >::value,
            "invalid callable - expected callable in form bool(T, T)"
        );

    return as_functor(std::move(comparison_op_func));
}
/// test if a container is sorted and unique according to a comparison criteria (defaults to operator<)
/// NOTE: ComparisonOpT must establish 'strict weak ordering' https://en.cppreference.com/w/cpp/named_req/Compare
template <typename T, typename ComparisonOpT = std::less<typename T::value_type>>
bool is_sorted_and_unique(const T &container, ComparisonOpT comparison_op = ComparisonOpT{})
{
    using ValueT = typename T::value_type;
    static_assert(
            std::is_same<
                    bool,
                    decltype(
                        comparison_op(
                            std::declval<std::remove_cv_t<ValueT>>(),
                            std::declval<std::remove_cv_t<ValueT>>()
                        )
                    )
                >::value,
            "invalid callable - expected callable in form bool(ValueT, ValueT)"
        );

    if (!std::is_sorted(container.begin(), container.end(), comparison_op))
        return false;

    if (std::adjacent_find(container.begin(),
                container.end(),
                [comparison_op = std::move(comparison_op)](const ValueT &a, const ValueT &b) -> bool
                {
                    return !comparison_op(a, b) && !comparison_op(b, a);
                })
            != container.end())
        return false;

    return true;
}
/// specialization for raw function pointers
template <typename T>
bool is_sorted_and_unique(const T &container,
    bool (*const comparison_op_func)(const typename T::value_type&, const typename T::value_type&))
{
    return is_sorted_and_unique(container, compare_func<typename T::value_type>(comparison_op_func));
}
/// convenience wrapper for checking if the key to a mapped object is correct for that object
/// example: std::unorderd_map<rct::key, std::pair<rct::key, rct::xmr_amount>> where the map key is supposed to
///   reproduce the pair's rct::key; use the predicate to check that relationship
template <typename KeyT, typename ValueT, typename PredT>
bool keys_match_internal_values(const std::unordered_map<KeyT, ValueT> &map, PredT check_key_func)
{
    static_assert(
            std::is_same<
                    bool,
                    decltype(
                        check_key_func(
                            std::declval<std::remove_cv_t<KeyT>>(),
                            std::declval<std::remove_cv_t<ValueT>>()
                        )
                    )
                >::value,
            "invalid callable - expected callable in form bool(KeyT, ValueT)"
        );

    for (const auto &map_element : map)
    {
        if (!check_key_func(map_element.first, map_element.second))
            return false;
    }

    return true;
}
/// convenience wrapper for getting the last element after emplacing back
template <typename ContainerT>
typename ContainerT::value_type& add_element(ContainerT &container)
{
    container.emplace_back();
    return container.back();
}
/// convenience erasor for unordered maps: std::erase_if(std::unordered_map) is C++20
template <typename KeyT, typename ValueT, typename PredT>
void for_all_in_map_erase_if(std::unordered_map<KeyT, ValueT> &map_inout, PredT predicate)
{
    using MapValueT = typename std::unordered_map<KeyT, ValueT>::value_type;
    static_assert(
            std::is_same<
                    bool,
                    decltype(predicate(std::declval<std::remove_cv_t<MapValueT>>()))
                >::value,
            "invalid callable - expected callable in form bool(Value)"
        );

    for (auto map_it = map_inout.begin(); map_it != map_inout.end();)
    {
        if (predicate(*map_it))
            map_it = map_inout.erase(map_it);
        else
            ++map_it;
    }
}

} //namespace tools
