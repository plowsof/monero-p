// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 

#pragma once

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <string>

#include "byte_slice.h"
#include "byte_stream.h"
#include "rpc/message_data_structs.h"

namespace cryptonote
{

namespace rpc
{

  class Message
  {
      virtual void doToJson(rapidjson::Writer<epee::byte_stream>& dest) const
      {}

    public:
      static const char* STATUS_OK;
      static const char* STATUS_RETRY;
      static const char* STATUS_FAILED;
      static const char* STATUS_BAD_REQUEST;
      static const char* STATUS_BAD_JSON;

      Message() : status(STATUS_OK), rpc_version(0) { }

      virtual ~Message() { }

      void toJson(rapidjson::Writer<epee::byte_stream>& dest) const;

      virtual void fromJson(const rapidjson::Value& val);

      std::string status;
      std::string error_details;
      uint32_t rpc_version;
  };

  class FullMessage
  {
    public:
      ~FullMessage() { }

      FullMessage(std::string&& json_string, bool request=false);

      std::string getRequestType() const;

      const rapidjson::Value& getMessage() const;

      rapidjson::Value getMessageCopy();

      const rapidjson::Value& getID() const;

      cryptonote::rpc::error getError();

      static epee::byte_slice getRequest(const std::string& request, const Message& message, unsigned id);
      static epee::byte_slice getResponse(const Message& message, const rapidjson::Value& id);
    private:

      FullMessage() = default;
      FullMessage(const FullMessage&) = delete;
      FullMessage& operator=(const FullMessage&) = delete;

      FullMessage(const std::string& request, Message* message);
      FullMessage(Message* message);

      std::string contents;
      rapidjson::Document doc;
  };


  // convenience functions for bad input
  epee::byte_slice BAD_REQUEST(const std::string& request);
  epee::byte_slice BAD_REQUEST(const std::string& request, const rapidjson::Value& id);

  epee::byte_slice BAD_JSON(const std::string& error_details);


}  // namespace rpc

}  // namespace cryptonote
