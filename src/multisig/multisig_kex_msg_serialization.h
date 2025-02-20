// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 

#pragma once

#include "crypto/crypto.h"
#include "serialization/containers.h"
#include "serialization/crypto.h"
#include "serialization/serialization.h"

#include <cstdint>
#include <vector>


namespace multisig
{
  /// round 1 kex message
  struct multisig_kex_msg_serializable_round1
  {
    // privkey stored in msg
    crypto::secret_key msg_privkey;
    // pubkey used to sign this msg
    crypto::public_key signing_pubkey;
    // message signature
    crypto::signature signature;

    BEGIN_SERIALIZE()
      FIELD(msg_privkey)
      FIELD(signing_pubkey)
      FIELD(signature)
    END_SERIALIZE()
  };

  /// general kex message (if round > 1)
  struct multisig_kex_msg_serializable_general
  {
    // key exchange round this msg was produced for
    std::uint32_t kex_round;
    // pubkeys stored in msg
    std::vector<crypto::public_key> msg_pubkeys;
    // pubkey used to sign this msg
    crypto::public_key signing_pubkey;
    // message signature
    crypto::signature signature;

    BEGIN_SERIALIZE()
      VARINT_FIELD(kex_round)
      FIELD(msg_pubkeys)
      FIELD(signing_pubkey)
      FIELD(signature)
    END_SERIALIZE()
  };
} //namespace multisig
