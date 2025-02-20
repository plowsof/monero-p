// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 

#include <boost/multiprecision/cpp_int.hpp>

void div128_64(uint64_t dividend_hi, uint64_t dividend_lo, uint64_t divisor, uint64_t* quotient_hi, uint64_t *quotient_lo, uint64_t *remainder_hi, uint64_t *remainder_lo)
{
  typedef boost::multiprecision::uint128_t uint128_t;

  uint128_t dividend = dividend_hi;
  dividend <<= 64;
  dividend |= dividend_lo;

  uint128_t q, r;
  divide_qr(dividend, uint128_t(divisor), q, r);

  *quotient_hi = ((q >> 64) & 0xffffffffffffffffull).convert_to<uint64_t>();
  *quotient_lo = (q & 0xffffffffffffffffull).convert_to<uint64_t>();
  if (remainder_hi)
    *remainder_hi = ((r >> 64) & 0xffffffffffffffffull).convert_to<uint64_t>();
  if (remainder_lo)
    *remainder_lo = (r & 0xffffffffffffffffull).convert_to<uint64_t>();
}
