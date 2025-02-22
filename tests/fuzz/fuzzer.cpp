// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 

#include <boost/program_options.hpp>
#include "include_base_utils.h"
#include "string_tools.h"
#include "common/command_line.h"
#include "common/util.h"
#include "fuzzer.h"

#ifndef OSSFUZZ

#if (!defined(__clang__) || (__clang__ < 5))
static int __AFL_LOOP(int)
{
  static int once = 0;
  if (once)
    return 0;
  once = 1;
  return 1;
}
#endif

int run_fuzzer(int argc, const char **argv, Fuzzer &fuzzer)
{
  TRY_ENTRY();

  if (argc < 2)
  {
    std::cout << "usage: " << argv[0] << " " << "<filename>" << std::endl;
    return 1;
  }

#ifdef __AFL_HAVE_MANUAL_CONTROL
  __AFL_INIT();
#endif

  int ret = fuzzer.init();
  if (ret)
    return ret;

  const std::string filename = argv[1];
  while (__AFL_LOOP(1000))
  {
    ret = fuzzer.run(filename);
    if (ret)
      return ret;
  }

  return 0;

  CATCH_ENTRY_L0("run_fuzzer", 1);
}

#endif
