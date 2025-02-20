# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.
# 

set(EPEE_INCLUDE_DIR_BASE "${CMAKE_CURRENT_SOURCE_DIR}/../include")

# Add headers to the file list, to be able to search for them and autosave in IDEs.
monero_find_all_headers(EPEE_HEADERS_PUBLIC "${EPEE_INCLUDE_DIR_BASE}")

monero_add_library(epee byte_slice.cpp byte_stream.cpp hex.cpp abstract_http_client.cpp http_auth.cpp mlog.cpp net_helper.cpp net_utils_base.cpp string_tools.cpp parserse_base_utils.cpp
    wipeable_string.cpp levin_base.cpp memwipe.c connection_basic.cpp network_throttle.cpp network_throttle-detail.cpp mlocker.cpp buffer.cpp net_ssl.cpp
    int-util.cpp portable_storage.cpp
    misc_language.cpp
    file_io_utils.cpp
    net_parse_helpers.cpp
    ${EPEE_HEADERS_PUBLIC}
    )

if (USE_READLINE AND (GNU_READLINE_FOUND OR (DEPENDS AND NOT MINGW)))
  monero_add_library(epee_readline readline_buffer.cpp)
endif()

set_property(SOURCE memwipe.c PROPERTY C_STANDARD 11)

# Build and install libepee if we're building for GUI
if (BUILD_GUI_DEPS)
    if(IOS)
        set(lib_folder lib-${ARCH})
    else()
        set(lib_folder lib)
    endif()
    install(TARGETS epee
        ARCHIVE DESTINATION ${lib_folder})
    if (USE_READLINE AND GNU_READLINE_FOUND)
      install(TARGETS epee_readline
          ARCHIVE DESTINATION ${lib_folder})
    endif()
endif()

target_link_libraries(epee
  PUBLIC
    easylogging
    ${Boost_CHRONO_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    ${Boost_REGEX_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${OPENSSL_LIBRARIES}
  PRIVATE
    ${EXTRA_LIBRARIES})

if (USE_READLINE AND (GNU_READLINE_FOUND OR (DEPENDS AND NOT MINGW)))
  target_link_libraries(epee_readline
    PUBLIC
      easylogging
      ${Boost_SYSTEM_LIBRARY}
    PRIVATE
    ${GNU_READLINE_LIBRARY})
endif()

target_include_directories(epee
  PUBLIC
    "${EPEE_INCLUDE_DIR_BASE}"
    "${OPENSSL_INCLUDE_DIR}")

