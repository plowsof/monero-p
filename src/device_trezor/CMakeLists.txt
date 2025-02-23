# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.
#

set(TREZOR_PROTOB_H
        trezor/messages/messages.pb.h
        trezor/messages/messages-common.pb.h
        trezor/messages/messages-management.pb.h
        trezor/messages/messages-monero.pb.h
)

set(TREZOR_PROTOB_CPP
        trezor/messages/messages.pb.cc
        trezor/messages/messages-common.pb.cc
        trezor/messages/messages-management.pb.cc
        trezor/messages/messages-monero.pb.cc
)

set(trezor_headers
        trezor/exceptions.hpp
        trezor/messages_map.hpp
        trezor/protocol.hpp
        trezor/transport.hpp
        device_trezor_base.hpp
        device_trezor.hpp
        trezor.hpp
        ${TREZOR_PROTOB_H}
)

set(trezor_sources
        trezor/messages_map.cpp
        trezor/protocol.cpp
        trezor/transport.cpp
        device_trezor_base.cpp
        device_trezor.cpp
        ${TREZOR_PROTOB_CPP}
)

set(trezor_private_headers)


# Protobuf and LibUSB processed by CheckTrezor
if(DEVICE_TREZOR_READY)
    message(STATUS "Trezor: support enabled")

    if(USE_DEVICE_TREZOR_DEBUG)
        list(APPEND trezor_headers trezor/debug_link.hpp trezor/messages/messages-debug.pb.h)
        list(APPEND trezor_sources trezor/debug_link.cpp trezor/messages/messages-debug.pb.cc)
        message(STATUS "Trezor: debugging enabled")
    endif()

    if(ANDROID)
        set(TREZOR_EXTRA_LIBRARIES "log")
    endif()

    monero_private_headers(device_trezor
            ${device_private_headers})

    monero_add_library(device_trezor
            ${trezor_sources}
            ${trezor_headers}
            ${trezor_private_headers})

    target_link_libraries(device_trezor
            PUBLIC
            device
            cncrypto
            ringct_basic
            cryptonote_core
            common
            ${sodium_LIBRARIES}
            ${Boost_CHRONO_LIBRARY}
            ${Protobuf_LIBRARY}
            ${TREZOR_LIBUSB_LIBRARIES}
            PRIVATE
            ${EXTRA_LIBRARIES}
            ${TREZOR_EXTRA_LIBRARIES})

else()
    message(STATUS "Trezor: support disabled")
    monero_private_headers(device_trezor)
    monero_add_library(device_trezor device_trezor.cpp)
    target_link_libraries(device_trezor PUBLIC cncrypto)
endif()
