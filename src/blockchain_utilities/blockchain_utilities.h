// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
//

#pragma once

#include "version.h"


// bounds checking is done before writing to buffer, but buffer size
// should be a sensible maximum
#define BUFFER_SIZE (2 * 1024 * 1024)
#define CHUNK_SIZE_WARNING_THRESHOLD 500000
#define NUM_BLOCKS_PER_CHUNK 1
#define BLOCKCHAIN_RAW "blockchain.raw"

