// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
//
//

#include "messages_map.hpp"
#include "messages/messages.pb.h"
#include "messages/messages-common.pb.h"
#include "messages/messages-management.pb.h"
#include "messages/messages-monero.pb.h"

#ifdef WITH_TREZOR_DEBUGGING
#include "messages/messages-debug.pb.h"
#endif

using namespace std;
using namespace hw::trezor;

namespace hw{
namespace trezor
{

  const char * TYPE_PREFIX = "MessageType_";
  const char * PACKAGES[] = {
      "hw.trezor.messages.",
      "hw.trezor.messages.common.",
      "hw.trezor.messages.management.",
#ifdef WITH_TREZOR_DEBUGGING
      "hw.trezor.messages.debug.",
#endif
      "hw.trezor.messages.monero."
  };

  google::protobuf::Message * MessageMapper::get_message(int wire_number) {
    return MessageMapper::get_message(static_cast<messages::MessageType>(wire_number));
  }

  google::protobuf::Message * MessageMapper::get_message(messages::MessageType wire_number) {
    const string &messageTypeName = hw::trezor::messages::MessageType_Name(wire_number);
    if (messageTypeName.empty()) {
      throw exc::EncodingException(std::string("Message descriptor not found: ") + std::to_string(wire_number));
    }

    string messageName = messageTypeName.substr(strlen(TYPE_PREFIX));
    return MessageMapper::get_message(messageName);
  }

  google::protobuf::Message * MessageMapper::get_message(const std::string & msg_name) {
    // Each package instantiation so lookup works
    hw::trezor::messages::common::Success::default_instance();
    hw::trezor::messages::management::Cancel::default_instance();
    hw::trezor::messages::monero::MoneroGetAddress::default_instance();

#ifdef WITH_TREZOR_DEBUGGING
    hw::trezor::messages::debug::DebugLinkDecision::default_instance();
#endif

    google::protobuf::Descriptor const * desc = nullptr;
    for(const string &text : PACKAGES){
      desc = google::protobuf::DescriptorPool::generated_pool()
          ->FindMessageTypeByName(text + msg_name);
      if (desc != nullptr){
        break;
      }
    }

    if (desc == nullptr){
      throw exc::EncodingException(std::string("Message not found: ") + msg_name);
    }

    google::protobuf::Message* message =
        google::protobuf::MessageFactory::generated_factory()
            ->GetPrototype(desc)->New();

    return message;

//    // CODEGEN way, fast
//    switch(wire_number){
//      case 501:
//        return new messages::monero::MoneroTransactionSignRequest();
//      default:
//        throw std::runtime_error("not implemented");
//    }
//
//    // CODEGEN message -> number: specification
//    //    messages::MessageType get_message_wire_number(const messages::monero::MoneroTransactionSignRequest * msg) { return 501; }
//    //    messages::MessageType get_message_wire_number(const messages::management::ping * msg)
//
  }

  messages::MessageType MessageMapper::get_message_wire_number(const google::protobuf::Message * msg){
    return MessageMapper::get_message_wire_number(msg->GetDescriptor()->name());
  }

  messages::MessageType MessageMapper::get_message_wire_number(const google::protobuf::Message & msg){
    return MessageMapper::get_message_wire_number(msg.GetDescriptor()->name());
  }

  messages::MessageType MessageMapper::get_message_wire_number(const std::string & msg_name){
    string enumMessageName = std::string(TYPE_PREFIX) + msg_name;

    messages::MessageType res;
    bool r = hw::trezor::messages::MessageType_Parse(enumMessageName, &res);
    if (!r){
      throw exc::EncodingException(std::string("Message ") + msg_name + " not found");
    }

    return res;
  }

}
}
