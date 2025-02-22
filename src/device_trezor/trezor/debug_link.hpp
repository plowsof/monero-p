// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
//
//

#ifndef MONERO_DEBUG_LINK_H
#define MONERO_DEBUG_LINK_H

#include "transport.hpp"
#include "messages/messages-debug.pb.h"


namespace hw {
namespace trezor {

  class DebugLink {
  public:

    DebugLink();
    virtual ~DebugLink();

    void init(std::shared_ptr<Transport> & transport);
    void close();

    std::shared_ptr<messages::debug::DebugLinkState> state();
    void input_word(const std::string & word);
    void input_button(bool button);
    void input_swipe(messages::debug::DebugLinkDecision_DebugSwipeDirection direction);
    void press_yes() { input_button(true); }
    void press_no() { input_button(false); }
    void stop();

    template<class t_message=messages::debug::DebugLinkState>
    std::shared_ptr<t_message> call(
        const google::protobuf::Message & req,
        const boost::optional<messages::MessageType> &resp_type = boost::none,
        bool no_wait = false)
    {
      BOOST_STATIC_ASSERT(boost::is_base_of<google::protobuf::Message, t_message>::value);

      m_transport->write(req);
      if (no_wait){
        return nullptr;
      }

      // Read the response
      std::shared_ptr<google::protobuf::Message> msg_resp;
      hw::trezor::messages::MessageType msg_resp_type;
      m_transport->read(msg_resp, &msg_resp_type);

      messages::MessageType required_type = resp_type ? resp_type.get() : MessageMapper::get_message_wire_number<t_message>();
      if (msg_resp_type == required_type) {
        return message_ptr_retype<t_message>(msg_resp);
      } else if (msg_resp_type == messages::MessageType_Failure){
        throw_failure_exception(dynamic_cast<messages::common::Failure*>(msg_resp.get()));
      } else {
        throw exc::UnexpectedMessageException(msg_resp_type, msg_resp);
      }
    };

  private:
    std::shared_ptr<Transport> m_transport;

  };

}
}

#endif //MONERO_DEBUG_LINK_H
