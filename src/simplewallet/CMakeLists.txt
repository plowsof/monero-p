# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.
#

set(simplewallet_sources
  simplewallet.cpp)

set(simplewallet_headers)

monero_find_all_headers(simplewallet_private_headers "${CMAKE_CURRENT_SOURCE_DIR}")

monero_private_headers(simplewallet
  ${simplewallet_private_headers})
monero_add_executable(simplewallet
  ${simplewallet_sources}
  ${simplewallet_headers}
  ${simplewallet_private_headers})
target_link_libraries(simplewallet
  PRIVATE
    wallet
    rpc_base
    cryptonote_core
    cncrypto
    common
    mnemonics
    ${EPEE_READLINE}
    qrcodegen
    version
    ${Boost_CHRONO_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_LOCALE_LIBRARY}
    ${ICU_LIBRARIES}
    ${Boost_THREAD_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
    ${GNU_READLINE_LIBRARY}
    ${EXTRA_LIBRARIES})
set_property(TARGET simplewallet
  PROPERTY
    OUTPUT_NAME "monero-wallet-cli")
install(TARGETS simplewallet DESTINATION bin)
