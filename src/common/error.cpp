// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

#include "error.h"

#include <string>

namespace
{
    struct category final : std::error_category
    {
        virtual const char* name() const noexcept override final
        {
            return "common_category()";
        }

        virtual std::string message(int value) const override final
        {
            switch (common_error(value))
            {
                case common_error::kInvalidArgument:
                    return make_error_code(std::errc::invalid_argument).message();
                case common_error::kInvalidErrorCode:
                    return "expect<T> was given an error value of zero";
                default:
                    break;
            }
            return "Unknown basic_category() value";
        }

        virtual std::error_condition default_error_condition(int value) const noexcept override final
        {
            // maps specific errors to generic `std::errc` cases.
            switch (common_error(value))
            {
                case common_error::kInvalidArgument:
                case common_error::kInvalidErrorCode:
                    return std::errc::invalid_argument;
                default:
                    break;
            }
            return std::error_condition{value, *this};
        }
    };
}

std::error_category const& common_category() noexcept
{
    static const category instance{};
    return instance;
}

