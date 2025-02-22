// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

//

#pragma once

#include <cstddef>
#include "crypto/wallet/ops.h"

namespace crypto {
  namespace wallet {
// if C functions defined from external/supercop - cmake generates crypto/wallet/ops.h
#if defined(monero_crypto_generate_key_derivation)
      inline
      bool generate_key_derivation(const public_key &tx_pub, const secret_key &view_sec, key_derivation &out)
      {
        return monero_crypto_generate_key_derivation(out.data, tx_pub.data, view_sec.data) == 0;
      }

      inline
      bool derive_subaddress_public_key(const public_key &output_pub, const key_derivation &d, std::size_t index, public_key &out)
      {
        ec_scalar scalar;
        derivation_to_scalar(d, index, scalar);
        return monero_crypto_generate_subaddress_public_key(out.data, output_pub.data, scalar.data) == 0;
      }
#else
    using ::crypto::generate_key_derivation;
    using ::crypto::derive_subaddress_public_key;
#endif
  }
}
