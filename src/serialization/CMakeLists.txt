# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.
#

set(serialization_sources
  json_object.cpp)

set(serialization_headers)

monero_find_all_headers(serialization_private_headers "${CMAKE_CURRENT_SOURCE_DIR}")

monero_private_headers(serialization
  ${serialization_private_headers})
monero_add_library(serialization
  ${serialization_sources}
  ${serialization_headers}
  ${serialization_private_headers})
target_link_libraries(serialization
  LINK_PRIVATE
    cryptonote_basic
    cryptonote_core
    cryptonote_protocol
    epee
    ${Boost_CHRONO_LIBRARY}
    ${Boost_REGEX_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    ${EXTRA_LIBRARIES})
add_dependencies(serialization
  version)

