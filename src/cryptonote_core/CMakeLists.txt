# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.
#

set(cryptonote_core_sources
  blockchain.cpp
  cryptonote_core.cpp
  tx_pool.cpp
  tx_sanity_check.cpp
  cryptonote_tx_utils.cpp
  tx_verification_utils.cpp
)

set(cryptonote_core_headers)

monero_find_all_headers(cryptonote_core_private_headers "${CMAKE_CURRENT_SOURCE_DIR}")

monero_private_headers(cryptonote_core
  ${cryptonote_core_private_headers})
monero_add_library(cryptonote_core
  ${cryptonote_core_sources}
  ${cryptonote_core_headers}
  ${cryptonote_core_private_headers})
target_link_libraries(cryptonote_core
  PUBLIC
    version
    common
    cncrypto
    blockchain_db
    ringct
    device
    hardforks
    ${Boost_DATE_TIME_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_SERIALIZATION_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
  PRIVATE
    ${EXTRA_LIBRARIES})
