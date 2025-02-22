// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

// 

#include <boost/algorithm/string.hpp>
#include <stdarg.h>
#include "misc_log_ex.h"
#include "file_io_utils.h"
#include "spawn.h"
#include "notify.h"

#undef MONERO_DEFAULT_LOG_CATEGORY
#define MONERO_DEFAULT_LOG_CATEGORY "notify"

namespace tools
{

/*
  TODO: 
  - Improve tokenization to handle paths containing whitespaces, quotes, etc.
  - Windows unicode support (implies implementing unicode command line parsing code)
*/
Notify::Notify(const char *spec)
{
  CHECK_AND_ASSERT_THROW_MES(spec, "Null spec");

  boost::split(args, spec, boost::is_any_of(" \t"), boost::token_compress_on);
  CHECK_AND_ASSERT_THROW_MES(args.size() > 0, "Failed to parse spec");
  if (strchr(spec, '\'') || strchr(spec, '\"') || strchr(spec, '\\'))
    MWARNING("A notification spec contains a quote or backslash: note that these are handled verbatim, which may not be the intent");
  filename = args[0];
  CHECK_AND_ASSERT_THROW_MES(epee::file_io_utils::is_file_exist(filename), "File not found: " << filename);
}

static void replace(std::vector<std::string> &v, const char *tag, const char *s)
{
  for (std::string &str: v)
    boost::replace_all(str, tag, s);
}

int Notify::notify(const char *tag, const char *s, ...) const
{
  std::vector<std::string> margs = args;

  replace(margs, tag, s);

  va_list ap;
  va_start(ap, s);
  while ((tag = va_arg(ap, const char*)))
  {
    s = va_arg(ap, const char*);
    replace(margs, tag, s);
  }
  va_end(ap);

  return tools::spawn(filename.c_str(), margs, false);
}

}
