# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.
#

set(daemon_sources
  command_parser_executor.cpp
  command_server.cpp
  daemon.cpp
  executor.cpp
  main.cpp
  rpc_command_executor.cpp
)

set(daemon_headers)

monero_find_all_headers(daemon_private_headers "${CMAKE_CURRENT_SOURCE_DIR}")

monero_private_headers(daemon
  ${daemon_private_headers})
monero_add_executable(daemon
  ${daemon_sources}
  ${daemon_headers}
  ${daemon_private_headers})
target_link_libraries(daemon
  PRIVATE
    rpc
    blockchain_db
    cryptonote_core
    cncrypto
    common
    p2p
    cryptonote_protocol
    daemonizer
    serialization
    daemon_rpc_server
    ${EPEE_READLINE}
    version
    ${Boost_CHRONO_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_REGEX_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
    PkgConfig::libzmq
    ${GNU_READLINE_LIBRARY}
    ${EXTRA_LIBRARIES}
    $<TARGET_NAME_IF_EXISTS:blocks>)
set_property(TARGET daemon
  PROPERTY
    OUTPUT_NAME "monerod")
install(TARGETS daemon DESTINATION bin)
