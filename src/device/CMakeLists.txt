# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.
#

set(device_sources
  device.cpp
  device_default.cpp
  log.cpp
  )

if(HIDAPI_FOUND)
  set(device_sources 
    ${device_sources} 
    device_ledger.cpp
    device_io_hid.cpp
    )
endif()

set(device_headers
  device.hpp
  device_io.hpp
  device_default.hpp
  device_cold.hpp
  log.hpp
 )

if(HIDAPI_FOUND)
  set(device_headers 
    ${device_headers} 
    device_ledger.hpp
    device_io_hid.hpp
    )
endif()

set(device_private_headers)


monero_private_headers(device
  ${device_private_headers})

monero_add_library(device
  ${device_sources}
  ${device_headers}
  ${device_private_headers})

target_link_libraries(device
  PUBLIC
    ${HIDAPI_LIBRARIES}
    cncrypto
    cryptonote_format_utils_basic
    ringct_basic
    wallet-crypto
    ${OPENSSL_CRYPTO_LIBRARIES}
    ${Boost_SERIALIZATION_LIBRARY}
  PRIVATE
    version
    ${EXTRA_LIBRARIES})
