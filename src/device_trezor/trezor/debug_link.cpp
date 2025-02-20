// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
//
//

#include "debug_link.hpp"

namespace hw{
namespace trezor{

  DebugLink::DebugLink(){

  }

  DebugLink::~DebugLink(){
    if (m_transport){
      close();
    }
  }

  void DebugLink::init(std::shared_ptr<Transport> & transport){
    CHECK_AND_ASSERT_THROW_MES(!m_transport, "Already initialized");
    m_transport = transport;
    m_transport->open();
  }

  void DebugLink::close(){
    CHECK_AND_ASSERT_THROW_MES(m_transport, "Not initialized");
    if (m_transport) m_transport->close();
  }

  std::shared_ptr<messages::debug::DebugLinkState> DebugLink::state(){
    return call<messages::debug::DebugLinkState>(
        messages::debug::DebugLinkGetState(),
        boost::make_optional(messages::MessageType_DebugLinkGetState));
  }

  void DebugLink::input_word(const std::string & word){
    messages::debug::DebugLinkDecision decision;
    decision.set_input(word);
    call(decision, boost::none, true);
  }

  void DebugLink::input_button(bool button){
    messages::debug::DebugLinkDecision decision;
    decision.set_button(button ? messages::debug::DebugLinkDecision_DebugButton_YES : messages::debug::DebugLinkDecision_DebugButton_NO);
    call(decision, boost::none, true);
  }

  void DebugLink::input_swipe(messages::debug::DebugLinkDecision_DebugSwipeDirection direction){
    messages::debug::DebugLinkDecision decision;
    decision.set_swipe(direction);
    call(decision, boost::none, true);
  }

  void DebugLink::stop(){
    messages::debug::DebugLinkStop msg;
    call(msg, boost::none, true);
  }





}
}
