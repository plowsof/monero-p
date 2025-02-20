#!/bin/bash -e

# Copyright (c) The Monero Project
# 
# This source code is licensed under the BSD-3 license found in the
# LICENSE file in the root directory of this source tree.
#

# clang-tidy runs lint checks on C & C++ sources and headers.
# Don't use this script directly but call clang-tidy-run-cc.sh or clang-tidy-run-cpp.sh instead

DIR_BUILD_BASE="build/clang-tidy"
RESULT_BASE="clang-tidy-result"

function tidy_for_language() {
	LANG="${1}"
	DIR_BUILD="${DIR_BUILD_BASE}-${LANG}"
	RESULT="${RESULT_BASE}-${LANG}.txt"

	mkdir -p "$DIR_BUILD" && pushd "$DIR_BUILD"
	rm `find . -name "CMakeCache.txt"` || true

	cmake ../.. \
	-DCMAKE_C_COMPILER=clang \
	-DCMAKE_CXX_COMPILER=clang++ \
	-DUSE_CCACHE=ON \
	-DUSE_CLANG_TIDY_${LANG}=ON \
	-DBUILD_SHARED_LIBS=ON \
	-DBUILD_TESTS=ON

	make clean 					# Clean up, so that the result can be regenerated from scratch
	time make -k 2>&1 | tee "$RESULT"		# Build and store the result. -k means: ignore errors
	#time make -k easylogging 2>&1 | tee "$RESULT"	# Quick testing: build a single target
	KPI=$(cat "$RESULT" | wc -l)
	tar -cJvf "$RESULT.txz" "$RESULT"		# Zip the result, because it's huge.
	rm -v "$RESULT"
	echo ""
	echo "Readable result stored in: $DIR_BUILD/$RESULT.txz"

	echo "$KPI" > "kpis.txt"

	popd
}
