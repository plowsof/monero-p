# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.
#

set(gen_ssl_cert_sources
  gen_ssl_cert.cpp)

monero_add_executable(gen_ssl_cert
  ${gen_ssl_cert_sources})
target_link_libraries(gen_ssl_cert
  PRIVATE
    common
    epee
    version
    ${EPEE_READLINE}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
    ${EXTRA_LIBRARIES})
add_dependencies(gen_ssl_cert
  version)
set_property(TARGET gen_ssl_cert
  PROPERTY
    OUTPUT_NAME "monero-gen-ssl-cert")
install(TARGETS gen_ssl_cert DESTINATION bin)
