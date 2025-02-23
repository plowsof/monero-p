# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.
#

if(APPLE)
  if(DEPENDS)
    list(APPEND EXTRA_LIBRARIES "-framework Foundation -framework ApplicationServices -framework AppKit -framework IOKit")
  else()
    find_library(IOKIT_LIBRARY IOKit)
    mark_as_advanced(IOKIT_LIBRARY)
    list(APPEND EXTRA_LIBRARIES ${IOKIT_LIBRARY})
  endif()
endif()

monero_add_library(cryptonote_format_utils_basic
  cryptonote_format_utils_basic.cpp
)
target_link_libraries(cryptonote_format_utils_basic
  PUBLIC
  cncrypto
)

set(cryptonote_basic_sources
  account.cpp
  connection_context.cpp
  cryptonote_basic_impl.cpp
  cryptonote_format_utils.cpp
  difficulty.cpp
  hardfork.cpp
  merge_mining.cpp
  miner.cpp)

set(cryptonote_basic_headers)

monero_find_all_headers(cryptonote_basic_private_headers "${CMAKE_CURRENT_SOURCE_DIR}")

monero_private_headers(cryptonote_basic
  ${cryptonote_basic_private_headers})
monero_add_library(cryptonote_basic
  ${cryptonote_basic_sources}
  ${cryptonote_basic_headers}
  ${cryptonote_basic_private_headers})
target_link_libraries(cryptonote_basic
  PUBLIC
    common
    cncrypto
    checkpoints
    cryptonote_format_utils_basic
    device
    ${Boost_DATE_TIME_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_SERIALIZATION_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
  PRIVATE
    ${EXTRA_LIBRARIES})
