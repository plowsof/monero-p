// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

// 

#pragma once 

#include <cctype>
#include <cwchar>
#include <stdexcept>

namespace tools
{
  template<typename T, typename Transform>
  inline T utf8canonical(const T &s, Transform t = [](wint_t c)->wint_t { return c; })
  {
    T sc = "";
    size_t avail = s.size();
    const char *ptr = s.data();
    wint_t cp = 0;
    int bytes = 1;
    char wbuf[8], *wptr;
    while (avail--)
    {
      if ((*ptr & 0x80) == 0)
      {
        cp = *ptr++;
        bytes = 1;
      }
      else if ((*ptr & 0xe0) == 0xc0)
      {
        if (avail < 1)
          throw std::runtime_error("Invalid UTF-8");
        cp = (*ptr++ & 0x1f) << 6;
        cp |= *ptr++ & 0x3f;
        --avail;
        bytes = 2;
      }
      else if ((*ptr & 0xf0) == 0xe0)
      {
        if (avail < 2)
          throw std::runtime_error("Invalid UTF-8");
        cp = (*ptr++ & 0xf) << 12;
        cp |= (*ptr++ & 0x3f) << 6;
        cp |= *ptr++ & 0x3f;
        avail -= 2;
        bytes = 3;
      }
      else if ((*ptr & 0xf8) == 0xf0)
      {
        if (avail < 3)
          throw std::runtime_error("Invalid UTF-8");
        cp = (*ptr++ & 0x7) << 18;
        cp |= (*ptr++ & 0x3f) << 12;
        cp |= (*ptr++ & 0x3f) << 6;
        cp |= *ptr++ & 0x3f;
        avail -= 3;
        bytes = 4;
      }
      else
        throw std::runtime_error("Invalid UTF-8");

      cp = t(cp);
      if (cp <= 0x7f)
        bytes = 1;
      else if (cp <= 0x7ff)
        bytes = 2;
      else if (cp <= 0xffff)
        bytes = 3;
      else if (cp <= 0x10ffff)
        bytes = 4;
      else
        throw std::runtime_error("Invalid code point UTF-8 transformation");

      wptr = wbuf;
      switch (bytes)
      {
        case 1: *wptr++ = cp; break;
        case 2: *wptr++ = 0xc0 | (cp >> 6); *wptr++ = 0x80 | (cp & 0x3f); break;
        case 3: *wptr++ = 0xe0 | (cp >> 12); *wptr++ = 0x80 | ((cp >> 6) & 0x3f); *wptr++ = 0x80 | (cp & 0x3f); break;
        case 4: *wptr++ = 0xf0 | (cp >> 18); *wptr++ = 0x80 | ((cp >> 12) & 0x3f); *wptr++ = 0x80 | ((cp >> 6) & 0x3f); *wptr++ = 0x80 | (cp & 0x3f); break;
        default: throw std::runtime_error("Invalid UTF-8");
      }
      *wptr = 0;
      sc.append(wbuf, bytes);
      cp = 0;
      bytes = 1;
    }
    return sc;
  }
}
