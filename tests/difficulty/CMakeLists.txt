# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.
#

set(difficulty_sources
  difficulty.cpp)

set(difficulty_headers)

monero_add_minimal_executable(difficulty-tests
  ${difficulty_sources}
  ${difficulty_headers})
target_link_libraries(difficulty-tests
  PRIVATE
    cryptonote_basic
    ${EXTRA_LIBRARIES})
set_property(TARGET difficulty-tests
  PROPERTY
    FOLDER "tests")

add_test(
  NAME    difficulty
  COMMAND difficulty-tests "${CMAKE_CURRENT_SOURCE_DIR}/data.txt")
add_test(
  NAME    wide_difficulty
  COMMAND ${PYTHON_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/wide_difficulty.py" "${PYTHON_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/gen_wide_data.py" "${CMAKE_CURRENT_BINARY_DIR}/difficulty-tests" "${CMAKE_CURRENT_BINARY_DIR}/wide_data.txt")
