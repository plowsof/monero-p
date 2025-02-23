# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.
#

cmake_minimum_required (VERSION 3.5)
project (monero CXX)

file(GLOB P2P *)
source_group(p2p FILES ${P2P})

#add_library(p2p ${P2P})

#monero_private_headers(p2p ${P2P})
monero_add_library(p2p ${P2P})
target_link_libraries(p2p
  PUBLIC
    version
    cryptonote_core
    net
    ${UPNP_LIBRARIES}
    ${Boost_CHRONO_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    ${Boost_SERIALIZATION_LIBRARY}
  PRIVATE
    ${EXTRA_LIBRARIES})
