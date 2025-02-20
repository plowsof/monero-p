# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.
#

set(block_weight_sources
  block_weight.cpp)

set(block_weight_headers)

monero_add_minimal_executable(block_weight
  ${block_weight_sources}
  ${block_weight_headers})
target_link_libraries(block_weight
  PRIVATE
    cryptonote_core
    blockchain_db
    ${EXTRA_LIBRARIES})

add_test(
  NAME    block_weight
  COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/compare.py ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/block_weight.py ${CMAKE_CURRENT_BINARY_DIR}/block_weight)
