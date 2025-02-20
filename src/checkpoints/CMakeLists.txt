# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.
#

if(APPLE)
  if(DEPENDS)
    list(APPEND EXTRA_LIBRARIES "-framework Foundation -framework ApplicationServices -framework AppKit -framework IOKit")
  else()
    find_library(IOKIT_LIBRARY IOKit)
    mark_as_advanced(IOKIT_LIBRARY)
    list(APPEND EXTRA_LIBRARIES ${IOKIT_LIBRARY})
  endif()
endif()

set(checkpoints_sources
  checkpoints.cpp)

set(checkpoints_headers)

monero_find_all_headers(checkpoints_private_headers "${CMAKE_CURRENT_SOURCE_DIR}")

monero_private_headers(checkpoints
  ${checkpoints_private_headers})
monero_add_library(checkpoints
  ${checkpoints_sources}
  ${checkpoints_headers}
  ${checkpoints_private_headers})
target_link_libraries(checkpoints
  PUBLIC
    common
    cncrypto
    ${Boost_DATE_TIME_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_SERIALIZATION_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
  PRIVATE
    ${EXTRA_LIBRARIES})
