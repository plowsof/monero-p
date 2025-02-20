# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.
#

set(performance_tests_sources
  main.cpp)

set(performance_tests_headers
  check_tx_signature.h
  check_hash.h
  cn_slow_hash.h
  construct_tx.h
  derive_public_key.h
  derive_secret_key.h
  ge_frombytes_vartime.h
  generate_key_derivation.h
  generate_key_image.h
  generate_key_image_helper.h
  generate_keypair.h
  signature.h
  is_out_to_acc.h
  out_can_be_to_acc.h
  subaddress_expand.h
  range_proof.h
  bulletproof.h
  bulletproof_plus.h
  crypto_ops.h
  sc_reduce32.h
  sc_check.h
  multiexp.h
  multi_tx_test_base.h
  performance_tests.h
  performance_utils.h
  single_tx_test_base.h)

monero_add_minimal_executable(performance_tests
  ${performance_tests_sources}
  ${performance_tests_headers})
target_link_libraries(performance_tests
  PRIVATE
    wallet
    cryptonote_core
    common
    cncrypto
    epee
    seraphis_crypto
    ${Boost_CHRONO_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
    ${EXTRA_LIBRARIES})
set_property(TARGET performance_tests
  PROPERTY
    FOLDER "tests")
