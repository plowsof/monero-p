# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.
# 

cmake_minimum_required(VERSION 3.5)

project(easylogging CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

monero_enable_coverage()

find_package(Threads)
find_package(Backtrace)

monero_find_all_headers(EASYLOGGING_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}")

add_library(easylogging
  easylogging++.cc
  ${EASYLOGGING_HEADERS}
  )

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
include_directories("${CMAKE_CURRENT_BINARY_DIR}")
target_link_libraries(easylogging
  PRIVATE
    ${CMAKE_THREAD_LIBS_INIT}
    ${Backtrace_LIBRARIES})

# GUI/libwallet install target
if (BUILD_GUI_DEPS)
    if(IOS)
        set(lib_folder lib-${ARCH})
    else()
        set(lib_folder lib)
    endif()
    install(TARGETS easylogging
        ARCHIVE DESTINATION ${lib_folder}
        LIBRARY DESTINATION ${lib_folder})
endif()
set_property(TARGET easylogging APPEND PROPERTY COMPILE_FLAGS "-fPIC")

