# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.
#

set(core_tests_sources
  block_reward.cpp
  block_validation.cpp
  chain_split_1.cpp
  chain_switch_1.cpp
  chaingen.cpp
  chaingen001.cpp
  chaingen_main.cpp
  double_spend.cpp
  integer_overflow.cpp
  multisig.cpp
  ring_signature_1.cpp
  transaction_tests.cpp
  tx_validation.cpp
  tx_pool.cpp
  v2_tests.cpp
  rct.cpp
  bulletproofs.cpp
  bulletproof_plus.cpp
  rct2.cpp
  wallet_tools.cpp)

set(core_tests_headers
  block_reward.h
  block_validation.h
  chain_split_1.h
  chain_switch_1.h
  chaingen.h
  chaingen_tests_list.h
  double_spend.h
  double_spend.inl
  integer_overflow.h
  multisig.h
  ring_signature_1.h
  tx_pool.h
  transaction_tests.h
  tx_validation.h
  v2_tests.h
  rct.h
  bulletproofs.h
  bulletproof_plus.h
  rct2.h
  wallet_tools.h)

monero_add_minimal_executable(core_tests
  ${core_tests_sources}
  ${core_tests_headers})
target_link_libraries(core_tests
  PRIVATE
    multisig
    cryptonote_core
    p2p
    version
    epee
    device
    wallet
    ${CMAKE_THREAD_LIBS_INIT}
    ${EXTRA_LIBRARIES})
enable_stack_trace(core_tests)
set_property(TARGET core_tests
  PROPERTY
    FOLDER "tests")

add_test(
  NAME    core_tests
  COMMAND core_tests --generate_and_play_test_data)
