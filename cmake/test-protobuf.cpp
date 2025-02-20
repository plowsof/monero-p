// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 

#include <string>
#include <iostream>
#include <google/protobuf/message.h>
#include <google/protobuf/unknown_field_set.h>
#include "test-protobuf.pb.h"

int main(int argc, char *argv[]) {
  google::protobuf::UnknownFieldSet ufs;
  ufs.ClearAndFreeMemory();

  Success sc;
  sc.set_message("test");
  sc.SerializeToOstream(&std::cerr);
  return 0;
}
