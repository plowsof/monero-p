# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.
#

if(MINGW)
  set(daemonizer_sources
    windows_service.cpp
    windows_daemonizer.inl
  )
else()
  set(daemonizer_sources
    posix_fork.cpp
    posix_daemonizer.inl
  )
endif()

set(daemonizer_headers
)

if(MINGW)
  set(daemonizer_private_headers
    daemonizer.h
    windows_service.h
    windows_service_runner.h
  )
else()
  set(daemonizer_private_headers
    daemonizer.h
    posix_fork.h
  )
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions(-DDEBUG_TMPDIR_LOG=1)
endif()

monero_private_headers(daemonizer
  ${daemonizer_private_headers})
monero_add_library(daemonizer
  ${daemonizer_sources}
  ${daemonizer_headers}
  ${daemonizer_private_headers})
target_link_libraries(daemonizer
  PUBLIC
    common
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
  PRIVATE
    ${EXTRA_LIBRARIES})
