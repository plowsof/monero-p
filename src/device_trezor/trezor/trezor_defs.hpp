// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
//
//

#ifndef USE_DEVICE_TREZOR
#define USE_DEVICE_TREZOR 1
#endif

// HAVE_TREZOR_READY set by cmake after protobuf messages
// were generated successfully and all minimal dependencies are met.
#ifndef DEVICE_TREZOR_READY
#undef  USE_DEVICE_TREZOR
#define USE_DEVICE_TREZOR 0
#endif

#if USE_DEVICE_TREZOR
#define WITH_DEVICE_TREZOR 1
#endif

#ifndef WITH_DEVICE_TREZOR
#undef WITH_DEVICE_TREZOR_LITE
#endif

#if defined(HAVE_TREZOR_LIBUSB) && USE_DEVICE_TREZOR
#define WITH_DEVICE_TREZOR_WEBUSB 1
#endif

// Enable / disable UDP in the enumeration
#ifndef USE_DEVICE_TREZOR_UDP
#define USE_DEVICE_TREZOR_UDP 1
#endif

// Enable / disable UDP in the enumeration for release
#ifndef USE_DEVICE_TREZOR_UDP_RELEASE
#define USE_DEVICE_TREZOR_UDP_RELEASE 0
#endif

#if USE_DEVICE_TREZOR_UDP && (USE_DEVICE_TREZOR_UDP_RELEASE || defined(TREZOR_DEBUG))
#define WITH_DEVICE_TREZOR_UDP 1
#endif

// Avoids protobuf undefined macro warning
#ifndef PROTOBUF_INLINE_NOT_IN_HEADERS
#define PROTOBUF_INLINE_NOT_IN_HEADERS 0
#endif

// Fixes gcc7 problem with minor macro defined clashing with minor() field.
#ifdef minor
#undef minor
#endif
