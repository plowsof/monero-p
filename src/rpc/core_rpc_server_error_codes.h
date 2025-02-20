// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 
// 
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#pragma  once 


#define CORE_RPC_ERROR_CODE_WRONG_PARAM           -1
#define CORE_RPC_ERROR_CODE_TOO_BIG_HEIGHT        -2
#define CORE_RPC_ERROR_CODE_TOO_BIG_RESERVE_SIZE  -3
#define CORE_RPC_ERROR_CODE_WRONG_WALLET_ADDRESS  -4
#define CORE_RPC_ERROR_CODE_INTERNAL_ERROR        -5
#define CORE_RPC_ERROR_CODE_WRONG_BLOCKBLOB       -6
#define CORE_RPC_ERROR_CODE_BLOCK_NOT_ACCEPTED    -7
#define CORE_RPC_ERROR_CODE_CORE_BUSY             -9
#define CORE_RPC_ERROR_CODE_WRONG_BLOCKBLOB_SIZE  -10
#define CORE_RPC_ERROR_CODE_UNSUPPORTED_RPC       -11
#define CORE_RPC_ERROR_CODE_MINING_TO_SUBADDRESS  -12
#define CORE_RPC_ERROR_CODE_REGTEST_REQUIRED      -13
#define CORE_RPC_ERROR_CODE_PAYMENT_REQUIRED      -14
#define CORE_RPC_ERROR_CODE_INVALID_CLIENT        -15
#define CORE_RPC_ERROR_CODE_PAYMENT_TOO_LOW       -16
#define CORE_RPC_ERROR_CODE_DUPLICATE_PAYMENT     -17
#define CORE_RPC_ERROR_CODE_STALE_PAYMENT         -18
#define CORE_RPC_ERROR_CODE_RESTRICTED            -19
#define CORE_RPC_ERROR_CODE_UNSUPPORTED_BOOTSTRAP -20
#define CORE_RPC_ERROR_CODE_PAYMENTS_NOT_ENABLED  -21

static inline const char *get_rpc_server_error_message(int64_t code)
{
  switch (code)
  {
    case CORE_RPC_ERROR_CODE_WRONG_PARAM: return "Invalid parameter";
    case CORE_RPC_ERROR_CODE_TOO_BIG_HEIGHT: return "Height is too large";
    case CORE_RPC_ERROR_CODE_TOO_BIG_RESERVE_SIZE: return "Reserve size is too large";
    case CORE_RPC_ERROR_CODE_WRONG_WALLET_ADDRESS: return "Wrong wallet address";
    case CORE_RPC_ERROR_CODE_INTERNAL_ERROR: return "Internal error";
    case CORE_RPC_ERROR_CODE_WRONG_BLOCKBLOB: return "Wrong block blob";
    case CORE_RPC_ERROR_CODE_BLOCK_NOT_ACCEPTED: return "Block not accepted";
    case CORE_RPC_ERROR_CODE_CORE_BUSY: return "Core is busy";
    case CORE_RPC_ERROR_CODE_WRONG_BLOCKBLOB_SIZE: return "Wrong block blob size";
    case CORE_RPC_ERROR_CODE_UNSUPPORTED_RPC: return "Unsupported RPC";
    case CORE_RPC_ERROR_CODE_MINING_TO_SUBADDRESS: return "Mining to subaddress is not supported";
    case CORE_RPC_ERROR_CODE_REGTEST_REQUIRED: return "Regtest mode required";
    case CORE_RPC_ERROR_CODE_PAYMENT_REQUIRED: return "Payment required";
    case CORE_RPC_ERROR_CODE_INVALID_CLIENT: return "Invalid client";
    case CORE_RPC_ERROR_CODE_PAYMENT_TOO_LOW: return "Payment too low";
    case CORE_RPC_ERROR_CODE_DUPLICATE_PAYMENT: return "Duplicate payment";
    case CORE_RPC_ERROR_CODE_STALE_PAYMENT: return "Stale payment";
    case CORE_RPC_ERROR_CODE_RESTRICTED: return "Parameters beyond restricted allowance";
    case CORE_RPC_ERROR_CODE_UNSUPPORTED_BOOTSTRAP: return "Command is unsupported in bootstrap mode";
    case CORE_RPC_ERROR_CODE_PAYMENTS_NOT_ENABLED: return "Payments not enabled";
    default: MERROR("Unknown error: " << code); return "Unknown error";
  }
}

