# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.
#

set(crypto_sources
  aesb.c
  blake256.c
  blake2b.c
  chacha.c
  crypto-ops-data.c
  crypto-ops.c
  crypto.cpp
  generators.cpp
  groestl.c
  hash-extra-blake.c
  hash-extra-groestl.c
  hash-extra-jh.c
  hash-extra-skein.c
  hash.c
  hmac-keccak.c
  jh.c
  keccak.c
  oaes_lib.c
  random.c
  skein.c
  slow-hash.c
  rx-slow-hash.c
  CryptonightR_JIT.c
  tree-hash.c)

if(ARCH_ID STREQUAL "i386" OR ARCH_ID STREQUAL "x86_64" OR ARCH_ID STREQUAL "x86-64" OR ARCH_ID STREQUAL "amd64")
list(APPEND crypto_sources CryptonightR_template.S)
endif()

include_directories(${RANDOMX_INCLUDE})

set(crypto_headers)

monero_find_all_headers(crypto_private_headers "${CMAKE_CURRENT_SOURCE_DIR}")

monero_private_headers(cncrypto
  ${crypto_private_headers})
monero_add_library(cncrypto
  ${crypto_sources}
  ${crypto_headers}
  ${crypto_private_headers})
target_link_libraries(cncrypto
  PUBLIC
    epee
    randomx
    ${Boost_SYSTEM_LIBRARY}
    ${sodium_LIBRARIES}
  PRIVATE
    ${EXTRA_LIBRARIES})

if (ARM)
  option(NO_OPTIMIZED_MULTIPLY_ON_ARM
	   "Compute multiply using generic C implementation instead of ARM ASM" OFF)
  if(NO_OPTIMIZED_MULTIPLY_ON_ARM)
    message(STATUS "Using generic C implementation for multiply")
    set_property(SOURCE slow-hash.c
      PROPERTY COMPILE_DEFINITIONS "NO_OPTIMIZED_MULTIPLY_ON_ARM")
  endif()
endif()

# Because of the way Qt works on android with JNI, the code does not live in the main android thread
# So this code runs with a 1 MB default stack size. 
# This will force the use of the heap for the allocation of the scratchpad
if (ANDROID OR IOS)
  if( BUILD_GUI_DEPS )
    add_definitions(-DFORCE_USE_HEAP=1)
  endif()
endif()

# cheat because cmake and ccache hate each other
set_property(SOURCE CryptonightR_template.S PROPERTY LANGUAGE C)
set_property(SOURCE CryptonightR_template.S PROPERTY XCODE_EXPLICIT_FILE_TYPE sourcecode.asm)

# Must be done last, because it references libraries in this directory
add_subdirectory(wallet)
