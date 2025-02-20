// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 

#include "include_base_utils.h"
#include "file_io_utils.h"
#include "cryptonote_basic/blobdatatype.h"
#include "cryptonote_basic/cryptonote_basic.h"
#include "cryptonote_basic/cryptonote_format_utils.h"
#include "wallet/wallet2.h"
#include "fuzzer.h"

static tools::wallet2 *wallet = NULL;

BEGIN_INIT_SIMPLE_FUZZER()
  static tools::wallet2 local_wallet;
  wallet = &local_wallet;

  static const char * const spendkey_hex = "f285d4ac9e66271256fc7cde0d3d6b36f66efff6ccd766706c408e86f4997a0d";
  crypto::secret_key spendkey;
  epee::string_tools::hex_to_pod(spendkey_hex, spendkey);

  wallet->init("", boost::none, "", 0, true, epee::net_utils::ssl_support_t::e_ssl_support_disabled);
  wallet->set_subaddress_lookahead(1, 1);
  wallet->generate("", "", spendkey, true, false);
END_INIT_SIMPLE_FUZZER()

BEGIN_SIMPLE_FUZZER()
  tools::wallet2::unsigned_tx_set exported_txs;
  binary_archive<false> ar{{buf, len}};
  ::serialization::serialize(ar, exported_txs);
  std::vector<tools::wallet2::pending_tx> ptx;
  bool success = wallet->sign_tx(exported_txs, "/tmp/cold-transaction-test-signed", ptx);
  std::cout << (success ? "signed" : "error") << std::endl;
END_SIMPLE_FUZZER()
