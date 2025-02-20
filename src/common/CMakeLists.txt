# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.
#

include_directories(SYSTEM ${OPENSSL_INCLUDE_DIR})

set(common_sources
  base58.cpp
  command_line.cpp
  dns_utils.cpp
  download.cpp
  error.cpp
  expect.cpp
  util.cpp
  i18n.cpp
  notify.cpp
  password.cpp
  perf_timer.cpp
  pruning.cpp
  spawn.cpp
  threadpool.cpp
  updates.cpp
  aligned.c
  timings.cc
  combinator.cpp)

if (STACK_TRACE)
  list(APPEND common_sources stack_trace.cpp)
endif()

set(common_headers)

monero_find_all_headers(common_private_headers "${CMAKE_CURRENT_SOURCE_DIR}")

monero_private_headers(common
  ${common_private_headers})
monero_add_library(common
  ${common_sources}
  ${common_headers}
  ${common_private_headers}
  DEPENDS generate_translations_header)
target_link_libraries(common
  PUBLIC
    cncrypto
    ${UNBOUND_LIBRARIES}
    ${LIBUNWIND_LIBRARIES}
    ${Boost_DATE_TIME_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    ${Boost_REGEX_LIBRARY}
    ${Boost_CHRONO_LIBRARY}
  PRIVATE
    ${OPENSSL_LIBRARIES}
    ${EXTRA_LIBRARIES})
target_include_directories(common
  PRIVATE
    ${Boost_INCLUDE_DIRS})

#monero_install_headers(common
#  ${common_headers})
