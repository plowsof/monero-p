// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 

#include <cinttypes>
#include <stdlib.h>
#include <chrono>
#include "include_base_utils.h"
#include "string_tools.h"
#include "rpc_payment_signature.h"

#undef MONERO_DEFAULT_LOG_CATEGORY
#define MONERO_DEFAULT_LOG_CATEGORY "daemon.rpc.payment"

#define TIMESTAMP_LEEWAY (60 * 1000000) /* 60 seconds, in microseconds */

namespace cryptonote
{
  std::string make_rpc_payment_signature(const crypto::secret_key &skey)
  {
    std::string s;
    crypto::public_key pkey;
    crypto::secret_key_to_public_key(skey, pkey);
    crypto::signature sig;
    const uint64_t now = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    char ts[17];
    int ret = snprintf(ts, sizeof(ts), "%16.16" PRIx64, now);
    CHECK_AND_ASSERT_MES(ret == 16, "", "snprintf failed");
    ts[16] = 0;
    CHECK_AND_ASSERT_MES(strlen(ts) == 16, "", "Invalid time conversion");
    crypto::hash hash;
    crypto::cn_fast_hash(ts, 16, hash);
    crypto::generate_signature(hash, pkey, skey, sig);
    s = epee::string_tools::pod_to_hex(pkey) + ts + epee::string_tools::pod_to_hex(sig);
    return s;
  }

  bool verify_rpc_payment_signature(const std::string &message, crypto::public_key &pkey, uint64_t &ts)
  {
    if (message.size() != 2 * sizeof(crypto::public_key) + 16 + 2 * sizeof(crypto::signature))
    {
      MDEBUG("Bad message size: " << message.size());
      return false;
    }
    const std::string pkey_string = message.substr(0, 2 * sizeof(crypto::public_key));
    const std::string ts_string = message.substr(2 * sizeof(crypto::public_key), 16);
    const std::string signature_string = message.substr(2 * sizeof(crypto::public_key) + 16);
    if (!epee::string_tools::hex_to_pod(pkey_string, pkey))
    {
      MDEBUG("Bad client id");
      return false;
    }
    crypto::signature signature;
    if (!epee::string_tools::hex_to_pod(signature_string, signature))
    {
      MDEBUG("Bad signature");
      return false;
    }
    crypto::hash hash;
    crypto::cn_fast_hash(ts_string.data(), 16, hash);
    if (!crypto::check_signature(hash, pkey, signature))
    {
      MDEBUG("signature does not verify");
      return false;
    }
    char *endptr = NULL;
    errno = 0;
    unsigned long long ull = strtoull(ts_string.c_str(), &endptr, 16);
    if (ull == ULLONG_MAX && errno == ERANGE)
    {
      MDEBUG("bad timestamp");
      return false;
    }
    ts = ull;
    const uint64_t now = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    if (ts > now + TIMESTAMP_LEEWAY)
    {
      MDEBUG("Timestamp is in the future");
      return false;
    }
    if (ts < now - TIMESTAMP_LEEWAY)
    {
      MDEBUG("Timestamp is too old");
      return false;
    }
    return true;
  }
}
