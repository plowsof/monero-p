# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.
#

set(cn_deserialize_sources
  cn_deserialize.cpp
  )

monero_add_executable(cn_deserialize
  ${cn_deserialize_sources}
  ${cn_deserialize_private_headers})

target_link_libraries(cn_deserialize
  LINK_PRIVATE
    cryptonote_core
    blockchain_db
    device
    ringct
    p2p
    epee
    ${CMAKE_THREAD_LIBS_INIT})

set_property(TARGET cn_deserialize
	PROPERTY
	OUTPUT_NAME "monero-utils-deserialize")


set(object_sizes_sources
  object_sizes.cpp
  )

monero_add_executable(object_sizes
  ${object_sizes_sources}
  ${object_sizes_private_headers})

target_link_libraries(object_sizes
  LINK_PRIVATE
    cryptonote_core
    blockchain_db
    p2p
    epee
    ${CMAKE_THREAD_LIBS_INIT})

set_property(TARGET object_sizes
	PROPERTY
	OUTPUT_NAME "monero-utils-object-sizes")


set(dns_checks_sources
  dns_checks.cpp
  )

monero_add_executable(dns_checks
  ${dns_checks_sources}
  ${dns_checks_private_headers})

target_link_libraries(dns_checks
  LINK_PRIVATE
    common
    epee
    version
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT})

set_property(TARGET dns_checks
	PROPERTY
	OUTPUT_NAME "monero-utils-dns-checks")

