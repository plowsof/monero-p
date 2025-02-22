// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
//

#pragma once
#include <string>

#ifndef WIN32

namespace posix {

void fork(const std::string & pidfile);

}

#endif
