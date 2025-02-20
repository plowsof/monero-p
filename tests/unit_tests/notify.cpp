// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

// 

#ifdef __GLIBC__
#include <sys/stat.h>
#endif

#include "gtest/gtest.h"

#include <boost/filesystem.hpp>

#include "misc_language.h"
#include "string_tools.h"
#include "file_io_utils.h"
#include "common/notify.h"

TEST(notify, works)
{
#ifdef __GLIBC__
  mode_t prevmode = umask(077);
#endif
  const char *tmp = getenv("TEMP");
  if (!tmp)
    tmp = "/tmp";
  static const char *filename = "monero-notify-unit-test-XXXXXX";
  const size_t len = strlen(tmp) + 1 + strlen(filename);
  std::unique_ptr<char[]> name_template_(new char[len + 1]);
  char *name_template = name_template_.get();
  ASSERT_TRUE(name_template != NULL);
  snprintf(name_template, len + 1, "%s/%s", tmp, filename);
  int fd = mkstemp(name_template);
#ifdef __GLIBC__
  umask(prevmode);
#endif
  ASSERT_TRUE(fd >= 0);
  close(fd);

  const std::string spec = epee::string_tools::get_current_module_folder() + "/test_notifier"
#ifdef _WIN32
      + ".exe"
#endif
      + " " + name_template + " %s";

  tools::Notify notify(spec.c_str());
  notify.notify("%s", "1111111111111111111111111111111111111111111111111111111111111111", NULL);

  bool ok = false;
  for (int i = 0; i < 10; ++i)
  {
    epee::misc_utils::sleep_no_w(100);

    std::string s;
    if (epee::file_io_utils::load_file_to_string(name_template, s))
    {
      if (s == "1111111111111111111111111111111111111111111111111111111111111111")
      {
        ok = true;
        break;
      }
    }
  }
  boost::filesystem::remove(name_template);
  ASSERT_TRUE(ok);
}
