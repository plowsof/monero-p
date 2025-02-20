# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.
#

set(trezor_tests_sources
        tools.cpp
        daemon.cpp
        trezor_tests.cpp
        ../core_tests/chaingen.cpp
        ../core_tests/wallet_tools.cpp)

set(trezor_tests_headers
        tools.h
        daemon.h
        trezor_tests.h
        ../core_tests/chaingen.h
        ../core_tests/wallet_tools.h)

monero_add_minimal_executable(trezor_tests
  ${trezor_tests_sources}
  ${trezor_tests_headers})

target_link_libraries(trezor_tests
  PRIVATE
    multisig
    cryptonote_core
    p2p
    version
    epee
    device
    device_trezor
    wallet
    wallet_api
    rpc
    cryptonote_protocol
    daemon_rpc_server
    ${Boost_CHRONO_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    PkgConfig::libzmq
    ${CMAKE_THREAD_LIBS_INIT}
    ${EXTRA_LIBRARIES})

enable_stack_trace(trezor_tests)
set_property(TARGET trezor_tests
  PROPERTY
    FOLDER "tests")

add_test(
  NAME    trezor_tests
  COMMAND trezor_tests)
