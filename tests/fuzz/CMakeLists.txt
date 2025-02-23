# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.
#

monero_add_minimal_executable(block_fuzz_tests block.cpp fuzzer.cpp)
target_link_libraries(block_fuzz_tests
  PRIVATE
    cryptonote_core
    p2p
    epee
    device
    ${CMAKE_THREAD_LIBS_INIT}
    ${EXTRA_LIBRARIES}
    $ENV{LIB_FUZZING_ENGINE})
set_property(TARGET block_fuzz_tests
  PROPERTY
    FOLDER "tests")

monero_add_minimal_executable(transaction_fuzz_tests transaction.cpp fuzzer.cpp)
target_link_libraries(transaction_fuzz_tests
  PRIVATE
    cryptonote_core
    p2p
    epee
    device
    ${CMAKE_THREAD_LIBS_INIT}
    ${EXTRA_LIBRARIES}
    $ENV{LIB_FUZZING_ENGINE})
set_property(TARGET transaction_fuzz_tests
  PROPERTY
    FOLDER "tests")

monero_add_minimal_executable(signature_fuzz_tests signature.cpp fuzzer.cpp)
target_link_libraries(signature_fuzz_tests
  PRIVATE
    wallet
    cryptonote_core
    p2p
    epee
    device
    ${CMAKE_THREAD_LIBS_INIT}
    ${EXTRA_LIBRARIES}
    $ENV{LIB_FUZZING_ENGINE})
set_property(TARGET signature_fuzz_tests
  PROPERTY
    FOLDER "tests")

monero_add_minimal_executable(cold-outputs_fuzz_tests cold-outputs.cpp fuzzer.cpp)
target_link_libraries(cold-outputs_fuzz_tests
  PRIVATE
    wallet
    cryptonote_core
    p2p
    epee
    device
    ${CMAKE_THREAD_LIBS_INIT}
    ${EXTRA_LIBRARIES}
    $ENV{LIB_FUZZING_ENGINE})
set_property(TARGET cold-outputs_fuzz_tests
  PROPERTY
    FOLDER "tests")

monero_add_minimal_executable(cold-transaction_fuzz_tests cold-transaction.cpp fuzzer.cpp)
target_link_libraries(cold-transaction_fuzz_tests
  PRIVATE
    wallet
    cryptonote_core
    p2p
    epee
    device
    ${CMAKE_THREAD_LIBS_INIT}
    ${EXTRA_LIBRARIES}
    $ENV{LIB_FUZZING_ENGINE})
set_property(TARGET cold-transaction_fuzz_tests
  PROPERTY
    FOLDER "tests")

monero_add_minimal_executable(load-from-binary_fuzz_tests load_from_binary.cpp fuzzer.cpp)
target_link_libraries(load-from-binary_fuzz_tests
  PRIVATE
    common
    epee
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
    ${EXTRA_LIBRARIES}
    $ENV{LIB_FUZZING_ENGINE})
set_property(TARGET load-from-binary_fuzz_tests
  PROPERTY
    FOLDER "tests")

monero_add_minimal_executable(load-from-json_fuzz_tests load_from_json.cpp fuzzer.cpp)
target_link_libraries(load-from-json_fuzz_tests
  PRIVATE
    common
    epee
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
    ${EXTRA_LIBRARIES}
    $ENV{LIB_FUZZING_ENGINE})
set_property(TARGET load-from-json_fuzz_tests
  PROPERTY
    FOLDER "tests")

monero_add_minimal_executable(base58_fuzz_tests base58.cpp fuzzer.cpp)
target_link_libraries(base58_fuzz_tests
  PRIVATE
    common
    epee
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
    ${EXTRA_LIBRARIES}
    $ENV{LIB_FUZZING_ENGINE})
set_property(TARGET base58_fuzz_tests
  PROPERTY
    FOLDER "tests")

monero_add_minimal_executable(parse-url_fuzz_tests parse_url.cpp fuzzer.cpp)
target_link_libraries(parse-url_fuzz_tests
  PRIVATE
    epee
    ${Boost_REGEX_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
    ${EXTRA_LIBRARIES}
    $ENV{LIB_FUZZING_ENGINE})
set_property(TARGET parse-url_fuzz_tests
  PROPERTY
    FOLDER "tests")

monero_add_minimal_executable(http-client_fuzz_tests http-client.cpp fuzzer.cpp)
target_link_libraries(http-client_fuzz_tests
  PRIVATE
    epee
    ${Boost_THREAD_LIBRARY}
    ${Boost_CHRONO_LIBRARY}
    ${Boost_REGEX_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
    ${EXTRA_LIBRARIES}
    $ENV{LIB_FUZZING_ENGINE})
set_property(TARGET http-client_fuzz_tests
  PROPERTY
    FOLDER "tests")

monero_add_minimal_executable(levin_fuzz_tests levin.cpp fuzzer.cpp)
target_link_libraries(levin_fuzz_tests
  PRIVATE
    common
    epee
    ${Boost_THREAD_LIBRARY}
    ${Boost_CHRONO_LIBRARY}
    ${Boost_REGEX_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
    ${EXTRA_LIBRARIES}
    $ENV{LIB_FUZZING_ENGINE})
set_property(TARGET levin_fuzz_tests
  PROPERTY
    FOLDER "tests")

monero_add_minimal_executable(bulletproof_fuzz_tests bulletproof.cpp fuzzer.cpp)
target_link_libraries(bulletproof_fuzz_tests
  PRIVATE
    common
    epee
    ${Boost_THREAD_LIBRARY}
    ${Boost_CHRONO_LIBRARY}
    ${Boost_REGEX_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
    ${EXTRA_LIBRARIES}
    $ENV{LIB_FUZZING_ENGINE})
set_property(TARGET bulletproof_fuzz_tests
  PROPERTY
    FOLDER "tests")

monero_add_minimal_executable(tx-extra_fuzz_tests tx-extra.cpp fuzzer.cpp)
target_link_libraries(tx-extra_fuzz_tests
  PRIVATE
    cryptonote_basic
    common
    epee
    ${Boost_THREAD_LIBRARY}
    ${Boost_CHRONO_LIBRARY}
    ${Boost_REGEX_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
    ${EXTRA_LIBRARIES}
    $ENV{LIB_FUZZING_ENGINE})
set_property(TARGET tx-extra_fuzz_tests
  PROPERTY
    FOLDER "tests")

monero_add_minimal_executable(utf8_fuzz_tests utf8.cpp fuzzer.cpp)
target_link_libraries(utf8_fuzz_tests
  PRIVATE
    common
    epee
    ${Boost_THREAD_LIBRARY}
    ${Boost_CHRONO_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
    ${EXTRA_LIBRARIES}
    $ENV{LIB_FUZZING_ENGINE})
