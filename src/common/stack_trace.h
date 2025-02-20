// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 

#ifndef MONERO_EXCEPTION_H
#define MONERO_EXCEPTION_H

#include <string>

namespace tools
{

void set_stack_trace_log(const std::string &log);
void log_stack_trace(const char *msg);

}  // namespace tools

#endif
