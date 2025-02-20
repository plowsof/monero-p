# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.
#

set(ringct_basic_sources
  rctOps.cpp
  rctTypes.cpp
  rctCryptoOps.c
  multiexp.cc
  bulletproofs.cc
  bulletproofs_plus.cc)

monero_find_all_headers(ringct_basic_private_headers "${CMAKE_CURRENT_SOURCE_DIR}")

monero_private_headers(ringct_basic
  ${crypto_private_headers})
monero_add_library(ringct_basic
  ${ringct_basic_sources}
  ${ringct_basic_private_headers})
target_link_libraries(ringct_basic
  PUBLIC
    common
    cncrypto
  PRIVATE
    ${OPENSSL_LIBRARIES}
    ${EXTRA_LIBRARIES})

set(ringct_sources
  rctSigs.cpp
)

set(ringct_headers)

set(ringct_private_headers
  rctSigs.h
)

monero_private_headers(ringct
  ${crypto_private_headers})
monero_add_library(ringct
  ${ringct_sources}
  ${ringct_headers}
  ${ringct_private_headers})
target_link_libraries(ringct
  PUBLIC
    common
    cncrypto
    cryptonote_basic
    device
  PRIVATE
    ${OPENSSL_LIBRARIES}
    ${EXTRA_LIBRARIES})
