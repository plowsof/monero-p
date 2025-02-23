# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.
# 

if(FREEBSD)
  add_definitions(-DMDB_DSYNC=O_SYNC)
endif()

if(ANDROID)
  add_definitions("-DANDROID=1")
endif()


set (lmdb_sources
mdb.c
midl.c)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

add_library(lmdb
  ${lmdb_sources})
target_link_libraries(lmdb
  PRIVATE
    ${CMAKE_THREAD_LIBS_INIT})
if(${ARCH_WIDTH} EQUAL 32)
  target_compile_definitions(lmdb
    PUBLIC -DMDB_VL32)
endif()

# GUI/libwallet install target
if (BUILD_GUI_DEPS)
    if(IOS)
        set(lib_folder lib-${ARCH})
    else()
        set(lib_folder lib)
    endif()
    install(TARGETS lmdb
        ARCHIVE DESTINATION ${lib_folder}
        LIBRARY DESTINATION ${lib_folder})
endif()
set_property(TARGET lmdb APPEND PROPERTY COMPILE_FLAGS "-fPIC")
