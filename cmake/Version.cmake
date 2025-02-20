# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.
# 

function (write_version tag)
  set(VERSIONTAG "${tag}" CACHE STRING "The tag portion of the Monero software version" FORCE)
  configure_file("${CMAKE_CURRENT_LIST_DIR}/../src/version.cpp.in" "${CMAKE_BINARY_DIR}/version.cpp")
endfunction ()

find_package(Git QUIET)
if ("$Format:$" STREQUAL "")
  # We're in a tarball; use hard-coded variables.
  set(VERSION_IS_RELEASE "true")
  write_version("release")
elseif (GIT_FOUND OR Git_FOUND)
  message(STATUS "Found Git: ${GIT_EXECUTABLE}")
  include(GitVersion)
  get_version_tag_from_git("${GIT_EXECUTABLE}")
  write_version("${VERSIONTAG}")
else()
  message(STATUS "WARNING: Git was not found!")
  set(VERSION_IS_RELEASE "false")
  write_version("unknown")
endif ()
add_custom_target(genversion ALL
  DEPENDS "${CMAKE_BINARY_DIR}/version.cpp")
