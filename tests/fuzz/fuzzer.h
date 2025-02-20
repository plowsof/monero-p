// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 

#include <string>
#include "file_io_utils.h"

#ifdef OSSFUZZ

#define BEGIN_INIT_SIMPLE_FUZZER() \
  static int init() \
  { \
    try \
    {

#define END_INIT_SIMPLE_FUZZER() \
    } \
    catch (const std::exception &e) \
    { \
      fprintf(stderr, "Exception: %s\n", e.what()); \
      return 1; \
    } \
    return 0; \
  }

#define BEGIN_SIMPLE_FUZZER() \
extern "C" { \
  int LLVMFuzzerTestOneInput(const uint8_t *buf, size_t len) \
  { \
    try \
    { \
      static bool first = true; \
      if (first) \
      { \
        if (init()) \
          return 1; \
        first = false; \
      } \

#define END_SIMPLE_FUZZER() \
    } \
    catch (const std::exception &e) \
    { \
      fprintf(stderr, "Exception: %s\n", e.what()); \
      delete el::base::elStorage; \
      el::base::elStorage = NULL; \
      return 0; \
    } \
    delete el::base::elStorage; \
    el::base::elStorage = NULL; \
    return 0; \
  } \
}

#else

class Fuzzer
{
public:
  virtual int init() { return 0; }
  virtual int run(const std::string &filename) = 0;
};

int run_fuzzer(int argc, const char **argv, Fuzzer &fuzzer);

#define BEGIN_INIT_SIMPLE_FUZZER() \
  class SimpleFuzzer: public Fuzzer \
  { \
    virtual int init() \
    { \
      try \
      {

#define END_INIT_SIMPLE_FUZZER() \
      } \
      catch (const std::exception &e) \
      { \
        fprintf(stderr, "Exception: %s\n", e.what()); \
        return 1; \
      } \
      return 0; \
    }

#define BEGIN_SIMPLE_FUZZER() \
    virtual int run(const std::string &filename) \
    { \
      try \
      { \
        std::string s; \
        if (!epee::file_io_utils::load_file_to_string(filename, s)) \
        { \
          std::cout << "Error: failed to load file " << filename << std::endl; \
          return 1; \
        } \
        const uint8_t *buf = (const uint8_t*)s.data(); \
        const size_t len = s.size(); \
        {

#define END_SIMPLE_FUZZER() \
        } \
      } \
      catch (const std::exception &e) \
      { \
        fprintf(stderr, "Exception: %s\n", e.what()); \
        delete el::base::elStorage; \
        el::base::elStorage = NULL; \
        return 0; \
      } \
      delete el::base::elStorage; \
      el::base::elStorage = NULL; \
      return 0; \
    } \
  }; \
  int main(int argc, const char **argv) \
  { \
    TRY_ENTRY(); \
    SimpleFuzzer fuzzer; \
    return run_fuzzer(argc, argv, fuzzer); \
    CATCH_ENTRY_L0("main", 1); \
  }

#endif
