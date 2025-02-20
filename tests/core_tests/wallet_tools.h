// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
//
//
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#pragma once

#include "chaingen.h"
#include "wallet/wallet2.h"
#include "wallet/api/wallet.h"

typedef struct {
  tools::wallet2::transfer_details * td;
  cryptonote::tx_source_entry * src;

  std::unordered_set<size_t> * selected_idx;
  std::unordered_set<crypto::key_image> * selected_kis;
  size_t ntrans;
  size_t iters;
  uint64_t sum;
  size_t cur_utxo;
} tx_source_info_crate_t;

typedef std::function<bool(const tx_source_info_crate_t &info, bool &abort)> fnc_accept_tx_source_t;

// Wallet friend, direct access to required fields and private methods
class wallet_accessor_test
{
public:
  static void set_account(tools::wallet2 * wallet, cryptonote::account_base& account);
  static void set_password(tools::wallet2 * wallet, const epee::wipeable_string & password) { wallet->setup_keys(password); }
  static tools::wallet2::transfer_container & get_transfers(tools::wallet2 * wallet) { return wallet->m_transfers; }
  static subaddresses_t & get_subaddresses(tools::wallet2 * wallet) { return wallet->m_subaddresses; }
  static void process_parsed_blocks(tools::wallet2 * wallet, uint64_t start_height, const std::vector<cryptonote::block_complete_entry> &blocks, const std::vector<tools::wallet2::parsed_block> &parsed_blocks, uint64_t& blocks_added);
};

// Wallet API friend, direct access to required fields and private methods
class WalletApiAccessorTest
{
public:
  static void allow_mismatched_daemon_version(Monero::WalletImpl * wallet, bool allow_mismatch) { wallet->m_wallet->allow_mismatched_daemon_version(allow_mismatch); }
};

class wallet_tools
{
public:
  static void gen_tx_src(size_t mixin, uint64_t cur_height, const tools::wallet2::transfer_details & td, cryptonote::tx_source_entry & src, block_tracker &bt, const boost::optional<fnc_accept_output_t>& fnc_accept = boost::none);
  static void gen_block_data(block_tracker &bt, const cryptonote::block *bl, const map_hash2tx_t & mtx, cryptonote::block_complete_entry &bche, tools::wallet2::parsed_block &parsed_block, uint64_t &height);
  static void compute_subaddresses(std::unordered_map<crypto::public_key, cryptonote::subaddress_index> &subaddresses, cryptonote::account_base & creds, size_t account, size_t minors);
  static void process_transactions(tools::wallet2 * wallet, const std::vector<test_event_entry>& events, const cryptonote::block& blk_head, block_tracker &bt, const boost::optional<crypto::hash>& blk_tail=boost::none);
  static void process_transactions(const std::vector<tools::wallet2*>& wallets, const std::vector<test_event_entry>& events, const cryptonote::block& blk_head, block_tracker &bt, const boost::optional<crypto::hash>& blk_tail=boost::none);
  static void process_transactions(tools::wallet2 * wallet, const std::vector<const cryptonote::block*>& blockchain, const map_hash2tx_t & mtx, block_tracker &bt);
  static void process_transactions(const std::vector<tools::wallet2*>& wallets, const std::vector<const cryptonote::block*>& blockchain, const map_hash2tx_t & mtx, block_tracker &bt);
  static bool fill_tx_sources(tools::wallet2 * wallet, std::vector<cryptonote::tx_source_entry>& sources, size_t mixin, const boost::optional<size_t>& num_utxo, const boost::optional<uint64_t>& min_amount, block_tracker &bt, std::vector<size_t> &selected, uint64_t cur_height, ssize_t offset=0, int step=1, const boost::optional<fnc_accept_tx_source_t>& fnc_accept=boost::none, const boost::optional<fnc_accept_output_t>& fnc_in_accept = boost::none);
};

cryptonote::account_public_address get_address(const tools::wallet2*);

bool construct_tx_to_key(cryptonote::transaction& tx, tools::wallet2 * from_wallet, const var_addr_t& to, uint64_t amount,
                         std::vector<cryptonote::tx_source_entry> &sources,
                         uint64_t fee, bool rct=false, rct::RangeProofType range_proof_type=rct::RangeProofBorromean, int bp_version = 0);

bool construct_tx_to_key(cryptonote::transaction& tx, tools::wallet2 * sender_wallet, const std::vector<cryptonote::tx_destination_entry>& destinations,
                         std::vector<cryptonote::tx_source_entry> &sources,
                         uint64_t fee, bool rct, rct::RangeProofType range_proof_type, int bp_version = 0);

bool construct_tx_rct(tools::wallet2 * sender_wallet,
                      std::vector<cryptonote::tx_source_entry>& sources,
                      const std::vector<cryptonote::tx_destination_entry>& destinations,
                      const boost::optional<cryptonote::account_public_address>& change_addr,
                      std::vector<uint8_t> extra, cryptonote::transaction& tx,
                      bool rct=false, rct::RangeProofType range_proof_type=rct::RangeProofBorromean, int bp_version = 0);
