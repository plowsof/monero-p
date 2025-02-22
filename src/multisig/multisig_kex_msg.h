// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 

#pragma once

#include "crypto/crypto.h"

#include <cstdint>
#include <vector>


namespace multisig
{
  ////
  // multisig key exchange message
  // - can parse and validate an input message
  // - can construct and sign a new message
  //
  // msg_content = kex_round | signing_pubkey | expand(msg_pubkeys) | OPTIONAL msg_privkey
  // msg_to_sign = versioning-domain-sep | msg_content
  // msg = versioning-domain-sep | b58(msg_content | crypto_sig[signing_privkey](msg_to_sign))
  //
  // note: round 1 messages will contain a private key (e.g. for the aggregate multisig private view key)
  ///
  class multisig_kex_msg final
  {
  //member types: none

  //constructors
  public:
    // default constructor
    multisig_kex_msg() = default;

    // construct from info
    multisig_kex_msg(const std::uint32_t round,
      const crypto::secret_key &signing_privkey,
      std::vector<crypto::public_key> msg_pubkeys,
      const crypto::secret_key &msg_privkey = crypto::null_skey);

    // construct from string
    multisig_kex_msg(std::string msg);

    // copy constructor: default

  //destructor: default
    ~multisig_kex_msg() = default;

  //overloaded operators: none

  //member functions
    // get msg string
    const std::string& get_msg() const { return m_msg; }
    // get kex round
    std::uint32_t get_round() const { return m_kex_round; }
    // get msg pubkeys
    const std::vector<crypto::public_key>& get_msg_pubkeys() const { return m_msg_pubkeys; }
    // get msg privkey
    const crypto::secret_key& get_msg_privkey() const { return m_msg_privkey; }
    // get msg signing pubkey
    const crypto::public_key& get_signing_pubkey() const { return m_signing_pubkey; }

  private:
    // msg_to_sign = versioning-domain-sep | kex_round | signing_pubkey | expand(msg_pubkeys) | OPTIONAL msg_privkey
    crypto::hash get_msg_to_sign() const;
    // set: msg string based on msg contents, signing pubkey based on input privkey
    void construct_msg(const crypto::secret_key &signing_privkey);
    // parse msg string into parts, validate contents and signature
    void parse_and_validate_msg();

  //member variables
  private:
    // message as string
    std::string m_msg;

    // key exchange round this msg was produced for
    std::uint32_t m_kex_round;
    // pubkeys stored in msg
    std::vector<crypto::public_key> m_msg_pubkeys;
    // privkey stored in msg (if kex round 1)
    crypto::secret_key m_msg_privkey;
    // pubkey used to sign this msg
    crypto::public_key m_signing_pubkey;
  };
} //namespace multisig
