// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 

#pragma once

extern "C"
{
#include "crypto-ops.h"
}
#include "crypto.h"

namespace crypto
{

public_key get_G();
public_key get_H();
ge_p3 get_G_p3();
ge_p3 get_H_p3();
ge_cached get_G_cached();
ge_cached get_H_cached();

} //namespace crypto
