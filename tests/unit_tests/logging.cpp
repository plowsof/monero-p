// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 
//
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#include <boost/filesystem.hpp>
#include "gtest/gtest.h"
#include "file_io_utils.h"
#include "misc_log_ex.h"

static std::string log_filename;

static void init()
{
  boost::filesystem::path p = boost::filesystem::temp_directory_path() / boost::filesystem::unique_path();
  log_filename = p.string();
  mlog_configure(log_filename, false, 0);
}

static void cleanup()
{
  // windows does not let files be deleted if still in use, so leave droppings there
#ifndef _WIN32
  boost::filesystem::remove(log_filename);
#endif
}

static size_t nlines(const std::string &str)
{
  size_t n = 0;
  for (const char *ptr = str.c_str(); *ptr; ++ptr)
    if (*ptr == '\n')
      ++n;
  return n;
}

static bool load_log_to_string(const std::string &filename, std::string &str)
{
  if (!epee::file_io_utils::load_file_to_string(filename, str))
    return false;
  for (const char *ptr = str.c_str(); *ptr; ++ptr)
  {
    if (*ptr == '\n')
    {
      std::string prefix = std::string(str.c_str(), ptr - str.c_str());
      if (prefix.find("New log categories:") != std::string::npos)
      {
        str = std::string(ptr + 1, strlen(ptr + 1));
        break;
      }
    }
  }
  return true;
}

static void log()
{
  MFATAL("fatal");
  MERROR("error");
  MWARNING("warning");
  MINFO("info");
  MDEBUG("debug");
  MTRACE("trace");

  MCINFO("a.b.c.d", "a.b.c.d");
  MCINFO("a.b.c.e", "a.b.c.e");
  MCINFO("global", "global");
  MCINFO("x.y.z", "x.y.z");
  MCINFO("y.y.z", "y.y.z");
  MCINFO("x.y.x", "x.y.x");
}

TEST(logging, no_logs)
{
  init();
  mlog_set_categories("");
  log();
  std::string str;
  ASSERT_TRUE(load_log_to_string(log_filename, str));
  ASSERT_TRUE(str == "");
  cleanup();
}

TEST(logging, default)
{
  init();
  log();
  std::string str;
  ASSERT_TRUE(load_log_to_string(log_filename, str));
  ASSERT_TRUE(str.find("global") != std::string::npos);
  ASSERT_TRUE(str.find("fatal") != std::string::npos);
  ASSERT_TRUE(str.find("error") != std::string::npos);
  ASSERT_TRUE(str.find("debug") == std::string::npos);
  ASSERT_TRUE(str.find("trace") == std::string::npos);
  cleanup();
}

TEST(logging, all)
{
  init();
  mlog_set_categories("*:TRACE");
  log();
  std::string str;
  ASSERT_TRUE(load_log_to_string(log_filename, str));
  ASSERT_TRUE(str.find("global") != std::string::npos);
  ASSERT_TRUE(str.find("fatal") != std::string::npos);
  ASSERT_TRUE(str.find("error") != std::string::npos);
  ASSERT_TRUE(str.find("debug") != std::string::npos);
  ASSERT_TRUE(str.find("trace") != std::string::npos);
  cleanup();
}

TEST(logging, glob_suffix)
{
  init();
  mlog_set_categories("x.y*:TRACE");
  log();
  std::string str;
  ASSERT_TRUE(load_log_to_string(log_filename, str));
  ASSERT_TRUE(str.find("global") == std::string::npos);
  ASSERT_TRUE(str.find("x.y.z") != std::string::npos);
  ASSERT_TRUE(str.find("x.y.x") != std::string::npos);
  ASSERT_TRUE(str.find("y.y.z") == std::string::npos);
  cleanup();
}

TEST(logging, glob_prefix)
{
  init();
  mlog_set_categories("*y.z:TRACE");
  log();
  std::string str;
  ASSERT_TRUE(load_log_to_string(log_filename, str));
  ASSERT_TRUE(str.find("global") == std::string::npos);
  ASSERT_TRUE(str.find("x.y.z") != std::string::npos);
  ASSERT_TRUE(str.find("x.y.x") == std::string::npos);
  ASSERT_TRUE(str.find("y.y.z") != std::string::npos);
  cleanup();
}

TEST(logging, last_precedence)
{
  init();
  mlog_set_categories("global:FATAL,glo*:DEBUG");
  log();
  std::string str;
  ASSERT_TRUE(load_log_to_string(log_filename, str));
  ASSERT_TRUE(nlines(str) == 1);
  ASSERT_TRUE(str.find("global") != std::string::npos);
  ASSERT_TRUE(str.find("x.y.z") == std::string::npos);
  ASSERT_TRUE(str.find("x.y.x") == std::string::npos);
  ASSERT_TRUE(str.find("y.y.z") == std::string::npos);
  cleanup();
}

TEST(logging, multiline)
{
  init();
  mlog_set_categories("global:INFO");
  MGINFO("first\nsecond\nthird");
  std::string str;
  ASSERT_TRUE(load_log_to_string(log_filename, str));
  ASSERT_TRUE(nlines(str) == 3);
  ASSERT_TRUE(str.find("global") != std::string::npos);
  ASSERT_TRUE(str.find("first") != std::string::npos);
  ASSERT_TRUE(str.find("second") != std::string::npos);
  ASSERT_TRUE(str.find("third") != std::string::npos);
  ASSERT_TRUE(str.find("first\nsecond") == std::string::npos);
  ASSERT_TRUE(str.find("second\nthird") == std::string::npos);
  cleanup();
}

// These operations might segfault
TEST(logging, copy_ctor_segfault)
{
    const el::Logger log1("id1", nullptr);
    const el::Logger log2(log1);
}

TEST(logging, operator_equals_segfault)
{
    const el::Logger log1("id1", nullptr);
    el::Logger log2("id2", nullptr);
    log2 = log1;
}

TEST(logging, empty_configurations_throws)
{
    el::Logger log1("id1", nullptr);
    const el::Configurations cfg;
    EXPECT_ANY_THROW(log1.configure(cfg));
}
