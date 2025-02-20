# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.
#

set(clt_sources
  clt.cpp)

set(clt_headers
  net_load_tests.h)

monero_add_minimal_executable(net_load_tests_clt
  ${clt_sources}
  ${clt_headers})
target_link_libraries(net_load_tests_clt
  PRIVATE
    p2p
    cryptonote_core
    epee
    ${GTEST_LIBRARIES}
    ${Boost_CHRONO_LIBRARY}
    ${Boost_DATE_TIME_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_THREAD_LIBARRY}
    ${CMAKE_THREAD_LIBS_INIT}
    ${EXTRA_LIBRARIES})

set(srv_sources
  srv.cpp)

set(srv_headers
  net_load_tests.h)

monero_add_minimal_executable(net_load_tests_srv
  ${srv_sources}
  ${srv_headers})
target_link_libraries(net_load_tests_srv
  PRIVATE
    p2p
    cryptonote_core
    epee
    ${GTEST_LIBRARIES}
    ${Boost_CHRONO_LIBRARY}
    ${Boost_DATE_TIME_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
    ${EXTRA_LIBRARIES})

set_property(TARGET net_load_tests_clt net_load_tests_srv
  PROPERTY
    FOLDER "tests")

set_property(TARGET net_load_tests_clt net_load_tests_srv APPEND_STRING
  PROPERTY
    COMPILE_FLAGS " -Wno-undef -Wno-sign-compare")
