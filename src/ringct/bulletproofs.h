// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 
//
// Adapted from Java code by Sarang Noether

#pragma once

#ifndef BULLETPROOFS_H
#define BULLETPROOFS_H

#include "rctTypes.h"

namespace rct
{

Bulletproof bulletproof_PROVE(const rct::key &v, const rct::key &gamma);
Bulletproof bulletproof_PROVE(uint64_t v, const rct::key &gamma);
Bulletproof bulletproof_PROVE(const rct::keyV &v, const rct::keyV &gamma);
Bulletproof bulletproof_PROVE(const std::vector<uint64_t> &v, const rct::keyV &gamma);
bool bulletproof_VERIFY(const Bulletproof &proof);
bool bulletproof_VERIFY(const std::vector<const Bulletproof*> &proofs);
bool bulletproof_VERIFY(const std::vector<Bulletproof> &proofs);

}

#endif
