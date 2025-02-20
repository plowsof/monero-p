# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.
#

# include (${PROJECT_SOURCE_DIR}/cmake/libutils.cmake)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(wallet_sources
  wallet2.cpp
  wallet_args.cpp
  ringdb.cpp
  node_rpc_proxy.cpp
  message_store.cpp
  message_transporter.cpp
)

monero_find_all_headers(wallet_private_headers "${CMAKE_CURRENT_SOURCE_DIR}")

monero_private_headers(wallet
  ${wallet_private_headers})
monero_add_library(wallet
  ${wallet_sources}
  ${wallet_private_headers})
target_link_libraries(wallet
  PUBLIC
    rpc_base
    multisig
    common
    cryptonote_core
    mnemonics
    device_trezor
    net
    ${LMDB_LIBRARY}
    ${Boost_CHRONO_LIBRARY}
    ${Boost_SERIALIZATION_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    ${Boost_REGEX_LIBRARY}
  PRIVATE
    ${EXTRA_LIBRARIES})

if(NOT IOS)
  set(wallet_rpc_sources
    wallet_rpc_server.cpp)

  set(wallet_rpc_headers)

  set(wallet_rpc_private_headers
    wallet_rpc_server.h)

  monero_private_headers(wallet_rpc_server
    ${wallet_rpc_private_headers})
  monero_add_executable(wallet_rpc_server
    ${wallet_rpc_sources}
    ${wallet_rpc_headers}
    ${wallet_rpc_private_headers})

  target_link_libraries(wallet_rpc_server
    PRIVATE
      wallet
      rpc_base
      cryptonote_core
      cncrypto
      common
      version
      daemonizer
      ${EPEE_READLINE}
      ${Boost_CHRONO_LIBRARY}
      ${Boost_PROGRAM_OPTIONS_LIBRARY}
      ${Boost_FILESYSTEM_LIBRARY}
      ${Boost_THREAD_LIBRARY}
      ${CMAKE_THREAD_LIBS_INIT}
      ${EXTRA_LIBRARIES})
  set_property(TARGET wallet_rpc_server
    PROPERTY
      OUTPUT_NAME "monero-wallet-rpc")
  install(TARGETS wallet_rpc_server DESTINATION bin)
endif()

add_subdirectory(api)
