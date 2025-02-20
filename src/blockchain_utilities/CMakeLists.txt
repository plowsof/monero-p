# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.
#

set(blockchain_import_sources
  blockchain_import.cpp
  bootstrap_file.cpp
  blocksdat_file.cpp
  )

set(blockchain_import_private_headers
  bootstrap_file.h
  blocksdat_file.h
  bootstrap_serialization.h
  )

monero_private_headers(blockchain_import
	  ${blockchain_import_private_headers})

set(blockchain_export_sources
  blockchain_export.cpp
  bootstrap_file.cpp
  blocksdat_file.cpp
  )

set(blockchain_export_private_headers
  bootstrap_file.h
  blocksdat_file.h
  bootstrap_serialization.h
  )

monero_private_headers(blockchain_export
	  ${blockchain_export_private_headers})


set(blockchain_blackball_sources
  blockchain_blackball.cpp
  )

set(blockchain_blackball_private_headers
  bootstrap_file.h
  blocksdat_file.h
  bootstrap_serialization.h
  )

monero_private_headers(blockchain_blackball
	  ${blockchain_blackball_private_headers})

set(blockchain_usage_sources
  blockchain_usage.cpp
  )

set(blockchain_usage_private_headers)

monero_private_headers(blockchain_usage
	  ${blockchain_usage_private_headers})



set(blockchain_prune_known_spent_data_sources
  blockchain_prune_known_spent_data.cpp
  )

set(blockchain_prune_known_spent_data_private_headers)

monero_private_headers(blockchain_prune_known_spent_data
	  ${blockchain_prune_known_spent_data_private_headers})



set(blockchain_prune_sources
  blockchain_prune.cpp
  )

set(blockchain_prune_private_headers)

monero_private_headers(blockchain_prune
	  ${blockchain_prune_private_headers})



set(blockchain_ancestry_sources
  blockchain_ancestry.cpp
  )

set(blockchain_ancestry_private_headers)

monero_private_headers(blockchain_ancestry
	  ${blockchain_ancestry_private_headers})



set(blockchain_depth_sources
  blockchain_depth.cpp
  )

set(blockchain_depth_private_headers)

monero_private_headers(blockchain_depth
	  ${blockchain_depth_private_headers})

set(blockchain_stats_sources
  blockchain_stats.cpp
  )

set(blockchain_stats_private_headers)

monero_private_headers(blockchain_stats
	  ${blockchain_stats_private_headers})


monero_add_executable(blockchain_import
  ${blockchain_import_sources}
  ${blockchain_import_private_headers})

target_link_libraries(blockchain_import
  PRIVATE
    cryptonote_core
    blockchain_db
    version
    epee
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
    ${EXTRA_LIBRARIES}
    $<TARGET_NAME_IF_EXISTS:blocks>)

if(ARCH_WIDTH)
  target_compile_definitions(blockchain_import
    PUBLIC -DARCH_WIDTH=${ARCH_WIDTH})
endif()

set_property(TARGET blockchain_import
	PROPERTY
	OUTPUT_NAME "monero-blockchain-import")
install(TARGETS blockchain_import DESTINATION bin)

monero_add_executable(blockchain_export
  ${blockchain_export_sources}
  ${blockchain_export_private_headers})

target_link_libraries(blockchain_export
  PRIVATE
    cryptonote_core
    blockchain_db
    version
    epee
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
    ${EXTRA_LIBRARIES})

set_property(TARGET blockchain_export
	PROPERTY
	OUTPUT_NAME "monero-blockchain-export")
install(TARGETS blockchain_export DESTINATION bin)

monero_add_executable(blockchain_blackball
  ${blockchain_blackball_sources}
  ${blockchain_blackball_private_headers})

target_link_libraries(blockchain_blackball
  PRIVATE
    wallet
    cryptonote_core
    blockchain_db
    version
    epee
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
    ${EXTRA_LIBRARIES})

set_property(TARGET blockchain_blackball
	PROPERTY
	OUTPUT_NAME "monero-blockchain-mark-spent-outputs")
install(TARGETS blockchain_blackball DESTINATION bin)


monero_add_executable(blockchain_usage
  ${blockchain_usage_sources}
  ${blockchain_usage_private_headers})

target_link_libraries(blockchain_usage
  PRIVATE
    cryptonote_core
    blockchain_db
    version
    epee
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
    ${EXTRA_LIBRARIES})

set_property(TARGET blockchain_usage
	PROPERTY
	OUTPUT_NAME "monero-blockchain-usage")
install(TARGETS blockchain_usage DESTINATION bin)

monero_add_executable(blockchain_ancestry
  ${blockchain_ancestry_sources}
  ${blockchain_ancestry_private_headers})

target_link_libraries(blockchain_ancestry
  PRIVATE
    cryptonote_core
    blockchain_db
    version
    epee
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
    ${EXTRA_LIBRARIES})

set_property(TARGET blockchain_ancestry
	PROPERTY
	OUTPUT_NAME "monero-blockchain-ancestry")
install(TARGETS blockchain_ancestry DESTINATION bin)

monero_add_executable(blockchain_depth
  ${blockchain_depth_sources}
  ${blockchain_depth_private_headers})

target_link_libraries(blockchain_depth
  PRIVATE
    cryptonote_core
    blockchain_db
    version
    epee
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
    ${EXTRA_LIBRARIES})

set_property(TARGET blockchain_depth
	PROPERTY
	OUTPUT_NAME "monero-blockchain-depth")
install(TARGETS blockchain_depth DESTINATION bin)

monero_add_executable(blockchain_stats
  ${blockchain_stats_sources}
  ${blockchain_stats_private_headers})

target_link_libraries(blockchain_stats
  PRIVATE
    cryptonote_core
    blockchain_db
    version
    epee
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
    ${EXTRA_LIBRARIES})

set_property(TARGET blockchain_stats
	PROPERTY
	OUTPUT_NAME "monero-blockchain-stats")
install(TARGETS blockchain_stats DESTINATION bin)

monero_add_executable(blockchain_prune_known_spent_data
  ${blockchain_prune_known_spent_data_sources}
  ${blockchain_prune_known_spent_data_private_headers})

target_link_libraries(blockchain_prune_known_spent_data
  PRIVATE
    cryptonote_core
    blockchain_db
    p2p
    version
    epee
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
    ${EXTRA_LIBRARIES})

set_property(TARGET blockchain_prune_known_spent_data
	PROPERTY
	OUTPUT_NAME "monero-blockchain-prune-known-spent-data")
install(TARGETS blockchain_prune_known_spent_data DESTINATION bin)

monero_add_executable(blockchain_prune
  ${blockchain_prune_sources}
  ${blockchain_prune_private_headers})

set_property(TARGET blockchain_prune
	PROPERTY
	OUTPUT_NAME "monero-blockchain-prune")
install(TARGETS blockchain_prune DESTINATION bin)

target_link_libraries(blockchain_prune
  PRIVATE
    cryptonote_core
    blockchain_db
    p2p
    version
    epee
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
    ${EXTRA_LIBRARIES})
