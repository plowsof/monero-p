# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.
#

set(hash_sources
  main.cpp)

set(hash_headers)

monero_add_minimal_executable(hash-tests
  ${hash_sources}
  ${hash_headers})
target_link_libraries(hash-tests
  PRIVATE
    common
    cncrypto
    ${EXTRA_LIBRARIES})
set_property(TARGET hash-tests
  PROPERTY
    FOLDER "tests")

foreach (hash IN ITEMS fast slow slow-1 slow-2 slow-4 tree extra-blake extra-groestl extra-jh extra-skein blake2b)
  add_test(
    NAME    "hash-${hash}"
    COMMAND hash-tests "${hash}" "${CMAKE_CURRENT_SOURCE_DIR}/tests-${hash}.txt")
endforeach ()

add_test(
  NAME    "hash-variant2-int-sqrt"
  COMMAND hash-tests "variant2_int_sqrt")
