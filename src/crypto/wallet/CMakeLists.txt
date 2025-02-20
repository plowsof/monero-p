# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.

#

#
# Possibly user defined values.
#
set(MONERO_WALLET_CRYPTO_LIBRARY "auto" CACHE STRING "Select a wallet crypto library")

#
# If the user specified "auto", detect best library defaulting to internal.
#
if (${MONERO_WALLET_CRYPTO_LIBRARY} STREQUAL "auto")
   monero_crypto_autodetect(AVAILABLE BEST)
  if (DEFINED BEST)
    message("Wallet crypto is using ${BEST} backend")
    set(MONERO_WALLET_CRYPTO_LIBRARY ${BEST})
  else ()
    message("Defaulting to internal crypto library for wallet")
    set(MONERO_WALLET_CRYPTO_LIBRARY "cn")
  endif ()
endif ()

#
# Configure library target "wallet-crypto" - clients will use this as a
# library dependency which in turn will depend on the crypto library selected.
#
if (${MONERO_WALLET_CRYPTO_LIBRARY} STREQUAL "cn")
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/empty.h.in ${MONERO_GENERATED_HEADERS_DIR}/crypto/wallet/ops.h)
  add_library(wallet-crypto ALIAS cncrypto)
else ()
  monero_crypto_generate_header(${MONERO_WALLET_CRYPTO_LIBRARY} "${MONERO_GENERATED_HEADERS_DIR}/crypto/wallet/ops.h")
  monero_crypto_get_target(${MONERO_WALLET_CRYPTO_LIBRARY} CRYPTO_TARGET)
  add_library(wallet-crypto $<TARGET_OBJECTS:${CRYPTO_TARGET}>)
  target_link_libraries(wallet-crypto cncrypto)
endif ()


