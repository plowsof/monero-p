// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 

#include <stdio.h>
#include "misc_language.h"
#include "misc_log_ex.h"
#include "string_tools.h"
#include "rpc/rpc_payment_signature.h"

int main(int argc, const char **argv)
{
  TRY_ENTRY();
  if (argc > 3)
  {
    fprintf(stderr, "usage: %s [<secret_key> [N]]\n", argv[0]);
    return 1;
  }

  crypto::secret_key skey;

  if (argc == 1)
  {
    crypto::public_key pkey;
    crypto::random32_unbiased((unsigned char*)skey.data);
    crypto::secret_key_to_public_key(skey, pkey);
    printf("%s %s\n", epee::to_hex::string({to_bytes(skey), 32}).c_str(), epee::string_tools::pod_to_hex(pkey).c_str());
    return 0;
  }

  if (!epee::string_tools::hex_to_pod(argv[1], skey))
  {
    fprintf(stderr, "invalid secret key\n");
    return 1;
  }
  uint32_t count = 1;
  if (argc == 3)
  {
    int i = atoi(argv[2]);
    if (i <= 0)
    {
      fprintf(stderr, "invalid count\n");
      return 1;
    }
    count = (uint32_t)i;
  }
  while (count--)
  {
    std::string signature = cryptonote::make_rpc_payment_signature(skey);
    epee::misc_utils::sleep_no_w(1);
    printf("%s\n", signature.c_str());
  }
  return 0;
  CATCH_ENTRY_L0("main()", 1);
}
