# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.
#

set(gen_multisig_sources
  gen_multisig.cpp)

monero_add_executable(gen_multisig
  ${gen_multisig_sources})
target_link_libraries(gen_multisig
  PRIVATE
    wallet
    cryptonote_core
    cncrypto
    common
    epee
    ${EPEE_READLINE}
    ${Boost_CHRONO_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
    ${GNU_READLINE_LIBRARY}
    ${EXTRA_LIBRARIES})
add_dependencies(gen_multisig
  version)
set_property(TARGET gen_multisig
  PROPERTY
    OUTPUT_NAME "monero-gen-trusted-multisig")
install(TARGETS gen_multisig DESTINATION bin)
