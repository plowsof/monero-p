// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 
// 
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#pragma once

#include <sstream>
#include "binary_archive.h"

namespace serialization {
  /*! creates a new archive with the passed blob and serializes it into v
   */
  template <class T>
    bool parse_binary(const std::string &blob, T &v)
    {
      binary_archive<false> iar{epee::strspan<std::uint8_t>(blob)};
      return ::serialization::serialize(iar, v);
    }

  /*! dumps the data in v into the blob string
   */
  template<class T>
    bool dump_binary(T& v, std::string& blob)
    {
      std::stringstream ostr;
      binary_archive<true> oar(ostr);
      bool success = ::serialization::serialize(oar, v);
      blob = ostr.str();
      return success && ostr.good();
    };

}
