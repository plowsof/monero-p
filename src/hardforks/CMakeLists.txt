# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.
#

set(hardforks_sources
  hardforks.cpp)

monero_find_all_headers(hardforks_headers "${CMAKE_CURRENT_SOURCE_DIR}")

set(hardforks_private_headers)

monero_private_headers(hardforks
  ${hardforks_private_headers})
monero_add_library(hardforks
  ${hardforks_sources}
  ${hardforks_headers}
  ${hardforks_private_headers})
target_link_libraries(hardforks
  PUBLIC
    version
  PRIVATE
    ${EXTRA_LIBRARIES})
