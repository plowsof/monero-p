# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.
#

set(functional_tests_sources
  main.cpp
  transactions_flow_test.cpp
  transactions_generation_from_blockchain.cpp)

set(functional_tests_headers
  transactions_flow_test.h
  transactions_generation_from_blockchain.h)

monero_add_minimal_executable(functional_tests
  ${functional_tests_sources}
  ${functional_tests_headers})
target_link_libraries(functional_tests
  PRIVATE
    cryptonote_core
    wallet
    common
    cncrypto
    epee
    ${Boost_REGEX_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
    ${EXTRA_LIBRARIES})

set(make_test_signature_sources
  make_test_signature.cc)

monero_add_minimal_executable(make_test_signature
  ${make_test_signature_sources})

target_link_libraries(make_test_signature
  PRIVATE
    rpc_base
    cncrypto
    epee
    ${CMAKE_THREAD_LIBS_INIT}
    ${EXTRA_LIBRARIES})

monero_add_minimal_executable(cpu_power_test cpu_power_test.cpp)
find_program(PYTHON3_FOUND python3 REQUIRED)

execute_process(COMMAND ${PYTHON3_FOUND} "-c" "import requests; import psutil; import monotonic; import zmq; import deepdiff; print('OK')" OUTPUT_VARIABLE REQUESTS_OUTPUT OUTPUT_STRIP_TRAILING_WHITESPACE)
if (REQUESTS_OUTPUT STREQUAL "OK")
  add_test(
    NAME    functional_tests_rpc
    COMMAND ${PYTHON3_FOUND} "${CMAKE_CURRENT_SOURCE_DIR}/functional_tests_rpc.py" "${PYTHON3_FOUND}" "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_BINARY_DIR}" all)
  add_test(
    NAME    check_missing_rpc_methods
    COMMAND ${PYTHON3_FOUND} "${CMAKE_CURRENT_SOURCE_DIR}/check_missing_rpc_methods.py" "${CMAKE_SOURCE_DIR}")
else()
  message(WARNING "functional_tests_rpc and check_missing_rpc_methods skipped, needs the 'requests', 'psutil', 'monotonic', 'zmq', and 'deepdiff' python modules")
  set(CTEST_CUSTOM_TESTS_IGNORE ${CTEST_CUSTOM_TESTS_IGNORE} functional_tests_rpc check_missing_rpc_methods)
endif()
