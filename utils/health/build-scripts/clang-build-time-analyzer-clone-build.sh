#!/bin/bash -e

# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.
#

# This script checkouts and builds ClangBuildAnalyzer.
# The result is put into bin directory

DIR_THIS="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

TAG="v1.2.0"
PROG="ClangBuildAnalyzer"
DIR_OUT="$DIR_THIS/../bin"
DIR_BUILD="build"

mkdir -p "$DIR_BUILD" && cd "$DIR_BUILD"
if [ ! -d "$PROG" ]; then
	git clone https://github.com/aras-p/$PROG.git
fi
cd "$PROG"
git checkout "$TAG"
mkdir -p build && cd build
cmake ..
make -j`nproc`

mkdir -p "$DIR_OUT"
cp -v "$PROG" "$DIR_OUT"
make clean # Clean the used space

