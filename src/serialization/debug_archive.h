// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 
// 
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#pragma once

#include "json_archive.h"
#include "variant.h"

template <bool W>
struct debug_archive : public json_archive<W> {
  typedef typename json_archive<W>::stream_type stream_type;

  debug_archive(stream_type &s) : json_archive<W>(s) { }
  stream_type& stream() { return this->stream_; }
};

template <class T>
static inline bool do_serialize(debug_archive<true> &ar, T &v)
{
    ar.begin_object();
    ar.tag(variant_serialization_traits<debug_archive<true>, T>::get_tag());
    do_serialize(static_cast<json_archive<true>&>(ar), v);
    ar.end_object();
    ar.stream() << std::endl;
    return true;
}
