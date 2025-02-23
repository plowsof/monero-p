# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.
#

set(crypto_sources
  crypto-ops-data.c
  crypto-ops.c
  crypto.cpp
  hash.c
  main.cpp
  random.c)

set(crypto_headers
  crypto-tests.h)

monero_add_minimal_executable(cncrypto-tests
  ${crypto_sources}
  ${crypto_headers})
target_link_libraries(cncrypto-tests
  PRIVATE
    common
    ${Boost_SYSTEM_LIBRARY}
    ${EXTRA_LIBRARIES})
set_property(TARGET cncrypto-tests
  PROPERTY
    FOLDER "tests")

add_test(
  NAME    cncrypto
  COMMAND cncrypto-tests "${CMAKE_CURRENT_SOURCE_DIR}/tests.txt")

monero_add_minimal_executable(cnv4-jit-tests cnv4-jit.c)
target_link_libraries(cnv4-jit-tests
  PRIVATE
    common
    ${EXTRA_LIBRARIES})
set_property(TARGET cnv4-jit-tests
  PROPERTY
    FOLDER "tests")

add_test(
  NAME    cnv4-jit
  COMMAND cnv4-jit-tests 1788000 1789000)
