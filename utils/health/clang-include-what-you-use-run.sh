#!/bin/bash -e

# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.
#

# Include What You Use analyses the complexity of your header hierarchy and proposes optimisations.
# User documentation:
# https://github.com/include-what-you-use/include-what-you-use/blob/master/README.md

# Build variables
PROG="include-what-you-use"
PROG_SHORT="iwyu"
DIR_BUILD="build/clang-$PROG_SHORT"

RESULT="$PROG_SHORT-result.txt"

if hash "$PROG"; then
	echo "Found: $PROG"
else
	echo "Couldn't find: $PROG"
	echo "Please run the below command to install $PROG:"
	echo "sudo apt install $PROG_SHORT"
	exit 1
fi

mkdir -p "$DIR_BUILD" && cd "$DIR_BUILD"
rm `find . -name "CMakeCache.txt"` || true

UWYU_COMMAND="$PROG;-Xiwyu;any;-Xiwyu;iwyu;-Xiwyu;args" # Copy-pasted from the user docs.
	
cmake ../.. \
-DCMAKE_C_COMPILER=clang \
-DCMAKE_CXX_COMPILER=clang++ \
-DUSE_CCACHE=ON \
-DCMAKE_C_INCLUDE_WHAT_YOU_USE="$UWYU_COMMAND" \
-DCMAKE_CXX_INCLUDE_WHAT_YOU_USE="$UWYU_COMMAND" \
-DBUILD_SHARED_LIBS=ON \
-DBUILD_TESTS=ON

make clean					# Clean up to generate the full report
time make -k 2>&1 | tee "$RESULT"		# Run the scan. -k means: ignore errors
#time make -k easylogging 2>&1 | tee $RESULT	# Quick testing: build a single target
KPI=$(cat "$RESULT" | wc -l)
tar -cJvf "$RESULT.txz" "$RESULT"		# Zip the result, because it's huge.
rm -v "$RESULT"

echo ""
echo "Readable result stored in: $DIR_BUILD/$RESULT.gz"

echo "$KPI" > "kpis.txt"
