#!/bin/bash -e

# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.
#

# clang-tidy runs lint checks on C & C++ sources and headers.
# Run this script from the source directory.

DIR_THIS="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
source "$DIR_THIS/clang-tidy-run-common.sh"

tidy_for_language "C"
