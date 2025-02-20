# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.
# 

# We aren't even going to check the system for an installed LMDB driver, as it is too
# critical a consensus component to rely on dynamically linked libraries
message(STATUS "Using ${ARCH_WIDTH}-bit LMDB from source tree")
add_subdirectory(liblmdb)
set(LMDB_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/liblmdb" CACHE STRING "LMDB Include path")
set(LMDB_LIBRARY "lmdb" CACHE STRING "LMDB Library name")
if(CMAKE_SYSTEM_NAME MATCHES "NetBSD")
	set_property(TARGET lmdb APPEND_STRING PROPERTY COMPILE_FLAGS " -D_SEM_SEMUN_UNDEFINED")
elseif(CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
        set_property(TARGET lmdb APPEND_STRING PROPERTY COMPILE_FLAGS " -D_WANT_SEMUN")
endif()
