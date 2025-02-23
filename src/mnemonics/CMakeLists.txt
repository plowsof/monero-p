# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.
#

set(mnemonics_sources
  electrum-words.cpp)

set(mnemonics_headers)

monero_find_all_headers(mnemonics_private_headers "${CMAKE_CURRENT_SOURCE_DIR}")

monero_private_headers(mnemonics
  ${mnemonics_private_headers})
monero_add_library(mnemonics
  ${mnemonics_sources}
  ${mnemonics_headers}
  ${mnemonics_private_headers})
target_link_libraries(mnemonics
  PUBLIC
    epee
    easylogging
    ${Boost_SYSTEM_LIBRARY}
  PRIVATE
    ${EXTRA_LIBRARIES})
