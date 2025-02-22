// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 

#pragma once

#ifndef BULLETPROOFS_PLUS_H
#define BULLETPROOFS_PLUS_H

#include "rctTypes.h"

namespace rct
{

BulletproofPlus bulletproof_plus_PROVE(const rct::key &v, const rct::key &gamma);
BulletproofPlus bulletproof_plus_PROVE(uint64_t v, const rct::key &gamma);
BulletproofPlus bulletproof_plus_PROVE(const rct::keyV &v, const rct::keyV &gamma);
BulletproofPlus bulletproof_plus_PROVE(const std::vector<uint64_t> &v, const rct::keyV &gamma);
bool bulletproof_plus_VERIFY(const BulletproofPlus &proof);
bool bulletproof_plus_VERIFY(const std::vector<const BulletproofPlus*> &proofs);
bool bulletproof_plus_VERIFY(const std::vector<BulletproofPlus> &proofs);

}

#endif
