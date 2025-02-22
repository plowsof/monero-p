// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 


#pragma once

#include "ringct/rctTypes.h"

#include <set>
#include <vector>

namespace cryptonote {

class transaction;
struct tx_source_entry;
struct tx_destination_entry;
struct account_keys;

}

namespace multisig {

namespace signing {

class CLSAG_context_t;

// number of parallel signing nonces to use per signer (2 nonces as in musig2 and FROST)
constexpr std::size_t kAlphaComponents = 2;

class tx_builder_ringct_t final {
private:
  // the tx builder has been initialized
  bool initialized;
  // the tx builder is 'reconstructing' a tx that has already been created using this object
  bool reconstruction;
  // cached: mu_P*(local keys and sender-receiver secret and subaddress material) + mu_C*(commitment-to-zero secret)
  // - these are only used for the initial building of a tx (not reconstructions)                                           
  rct::keyV cached_w;
  // contexts for making CLSAG challenges with multisig nonces
  std::vector<CLSAG_context_t> CLSAG_contexts;
public:
  tx_builder_ringct_t();
  ~tx_builder_ringct_t();

  // prepare an unsigned transaction (and get tx privkeys for outputs)
  bool init(
    const cryptonote::account_keys& account_keys,
    const std::vector<std::uint8_t>& extra,
    const std::uint32_t subaddr_account,
    const std::set<std::uint32_t>& subaddr_minor_indices,
    std::vector<cryptonote::tx_source_entry>& sources,
    std::vector<cryptonote::tx_destination_entry>& destinations,
    const cryptonote::tx_destination_entry& change,
    const rct::RCTConfig& rct_config,
    const bool use_rct,
    const bool reconstruction,
    crypto::secret_key& tx_secret_key,
    std::vector<crypto::secret_key>& tx_aux_secret_keys,
    crypto::secret_key& tx_secret_key_entropy,
    cryptonote::transaction& unsigned_tx
  );

  // get the first partial signature for the specified input ('source')
  bool first_partial_sign(
    const std::size_t source,
    const rct::keyV& total_alpha_G,
    const rct::keyV& total_alpha_H,
    const rct::keyV& alpha,
    rct::key& c_0,
    rct::key& s
  );

  // get intermediate partial signatures for all the inputs
  bool next_partial_sign(
    const rct::keyM& total_alpha_G,
    const rct::keyM& total_alpha_H,
    const rct::keyM& alpha,
    const rct::key& x,
    rct::keyV& c_0,
    rct::keyV& s
  );

  // finalize an unsigned transaction (add challenges and real responses to incomplete CLSAG signatures)
  static bool finalize_tx(
    const std::vector<cryptonote::tx_source_entry>& sources,
    const rct::keyV& c_0,
    const rct::keyV& s,
    cryptonote::transaction& unsigned_tx
  );
};

} //namespace signing

} //namespace multisig
