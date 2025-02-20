# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.
#

set(libwallet_api_tests_sources
  main.cpp
)

set(libwallet_api_tests_headers
  )

monero_add_minimal_executable(libwallet_api_tests
  ${libwallet_api_tests_sources}
  ${libwallet_api_tests_headers})

target_link_libraries(libwallet_api_tests
  PUBLIC
  device
  PRIVATE
    wallet_api
    wallet
    version
    epee
    ${Boost_CHRONO_LIBRARY}
    ${Boost_SERIALIZATION_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_LOCALE_LIBRARY}
    ${ICU_LIBRARIES}
    ${GTEST_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    ${EXTRA_LIBRARIES})

set_property(TARGET libwallet_api_tests
  PROPERTY
    FOLDER "tests")

set_property(TARGET libwallet_api_tests
  APPEND_STRING
  PROPERTY
    COMPILE_FLAGS " -Wno-undef -Wno-sign-compare")

add_test(
  NAME    libwallet_api_tests
  COMMAND libwallet_api_tests)
