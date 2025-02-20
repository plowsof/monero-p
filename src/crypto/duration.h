// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

//

#pragma once

#include <chrono>
#include "crypto/crypto.h"

namespace crypto
{
  //! Generate poisson distributed values in discrete `D` time units.
  template<typename D>
  struct random_poisson_duration
  {
    using result_type = D;                 //!< std::chrono::duration time unit precision
    using rep = typename result_type::rep; //!< Type used to represent duration value

    //! \param average for generated durations
    explicit random_poisson_duration(result_type average)
      : dist(average.count() < 0 ? 0 : average.count())
    {}

    //! Generate a crypto-secure random duration
    result_type operator()()
    {
      crypto::random_device rand{};
      return result_type{dist(rand)};
    }

  private:
    std::poisson_distribution<rep> dist;
  };

    /* A custom duration is used for subsecond precision because of the
       variance. If 5000 milliseconds is given, 95% of the values fall between
       4859ms-5141ms in 1ms increments (not enough time variance). Providing 1/4
       seconds would yield 95% of the values between 3s-7.25s in 1/4s
       increments. */

  //! Generate random durations with 1 second precision
  using random_poisson_seconds = random_poisson_duration<std::chrono::seconds>;
  //! Generate random duration with 1/4 second precision
  using random_poisson_subseconds =
    random_poisson_duration<std::chrono::duration<std::chrono::milliseconds::rep, std::ratio<1, 4>>>;
}
