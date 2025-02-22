#!/bin/bash -e

# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.
#

# ClangBuildAnalyzer is able to analyze the aggregate build time of particular headers.

DIR_THIS="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

# Build variables
PROG="ClangBuildAnalyzer"
PROG_PATH="$DIR_THIS/bin/$PROG"
DIR_BUILD="build/clang-build-analyser"

# ClangBuildAnalyzer variables
DIR_MONITORED="."
RESULT="cba-result.txt"
TRACE="cba-trace.txt"

if [ -f "$PROG_PATH" ]; then
	echo "Found: $PROG_PATH"
else
	echo "Couldn't find: $PROG_PATH"
	echo "Please run the below script to clone and build $PROG:"
	echo "$DIR_THIS/build-scripts/clang-build-time-analyzer-clone-build.sh"
	exit 1
fi

mkdir -p "$DIR_BUILD" && cd "$DIR_BUILD"

cmake ../.. \
-DCMAKE_C_COMPILER=clang \
-DCMAKE_CXX_COMPILER=clang++ \
-DUSE_CCACHE=OFF \
-DUSE_COMPILATION_TIME_PROFILER=ON \
-DBUILD_SHARED_LIBS=ON \
-DBUILD_TESTS=ON

make clean 				# Clean up, so that the trace can be regenerated from scratch
$PROG_PATH --start $DIR_MONITORED	# Start monitoring
time make 				# Build
#time make easylogging			# Quick testing: build a single target
$PROG_PATH --stop $DIR_MONITORED $TRACE	# Stop and output to trace file
$PROG_PATH --analyze $TRACE | tee $RESULT # Analyze the trace, and store it in a readable format
gzip -f $TRACE # Zip the trace, because it's huge. -f overwrites the previously generated trace

echo ""
echo "Readable result stored in: $DIR_BUILD/$RESULT"
echo "The trace (analyser's input data) in: $DIR_BUILD/$TRACE.gz"

