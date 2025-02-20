# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.
#

set(seraphis_crypto_sources
  dummy.cpp)

monero_find_all_headers(seraphis_crypto_headers, "${CMAKE_CURRENT_SOURCE_DIR}")

monero_add_library(seraphis_crypto
  ${seraphis_crypto_sources}
  ${seraphis_crypto_headers})

target_link_libraries(seraphis_crypto
  PUBLIC
    cncrypto
    common
    epee
    ringct
  PRIVATE
    ${EXTRA_LIBRARIES})

target_include_directories(seraphis_crypto
  PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
  PRIVATE
    ${Boost_INCLUDE_DIRS})
