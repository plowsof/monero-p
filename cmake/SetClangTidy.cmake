# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.
#

# https://cmake.org/cmake/help/latest/variable/CMAKE_LANG_CLANG_TIDY.html
# This module sets the following variables:
# CMAKE_C_CLANG_TIDY
# CMAKE_CXX_CLANG_TIDY
# when clang-tidy is found in PATH. Afterwards, the code is being linted by the tool.
# The checks to be enabled can be manipulated with the variable MONERO_CLANG_TIDY_CHECKS

macro (monero_clang_tidy LANGUAGE)
	set(TOOL_NAME "clang-tidy")
	set(MONERO_CLANG_TIDY_MIN_VERSION "3.6")
	if(${CMAKE_VERSION} VERSION_LESS "${MONERO_CLANG_TIDY_MIN_VERSION}")
		message(FATAL_ERROR "Sorry, ${TOOL_NAME} is available for CMake from version ${MONERO_CLANG_TIDY_MIN_VERSION}")
	else()
		message(STATUS "Trying to enable ${TOOL_NAME}")
		find_program(MONERO_CLANG_BIN ${TOOL_NAME})
		if(NOT MONERO_CLANG_BIN)
			message(FATAL_ERROR "${TOOL_NAME} not found! Try running: sudo apt install ${TOOL_NAME}")
		else()
			message(STATUS "Found ${MONERO_CLANG_BIN}")
			set(MONERO_CLANG_TIDY_CHECKS
			    -header-filter=.; 	# By default the headers are excluded. This line enables them.
			    -checks=*; 		# Currently enabling all checks
			    # An example of selectively enabling checks:
			    #-checks=bugprone-*,cppcoreguidelines-avoid-goto # Have to be in one line :(
			)
			# Current list of checks is avaibale under:
			# https://clang.llvm.org/extra/clang-tidy/
			if (${LANGUAGE} STREQUAL "C")
				set(CMAKE_C_CLANG_TIDY
					${MONERO_CLANG_BIN}; # Mind the semicolon
					${MONERO_CLANG_TIDY_CHECKS}
				)
			elseif (${LANGUAGE} STREQUAL "CXX")
				set(CMAKE_CXX_CLANG_TIDY
					${MONERO_CLANG_BIN}; # Mind the semicolon
					${MONERO_CLANG_TIDY_CHECKS}
				)
			else()
				message(FATAL_ERROR "${TOOL_NAME}: Unsupported language: ${LANGUAGE}")
			endif()
		endif()
	endif()
endmacro()

