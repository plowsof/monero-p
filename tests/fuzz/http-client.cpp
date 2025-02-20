// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 

#include <boost/utility/string_ref.hpp>
#include "include_base_utils.h"
#include "file_io_utils.h"
#include "net/http_client.h"
#include "net/net_ssl.h"
#include "fuzzer.h"

class dummy_client
{
public:
  bool connect(const std::string& addr, int port, std::chrono::milliseconds timeout, bool ssl = false, const std::string& bind_ip = "0.0.0.0") { return true; }
  bool connect(const std::string& addr, const std::string& port, std::chrono::milliseconds timeout, bool ssl = false, const std::string& bind_ip = "0.0.0.0") { return true; }
  bool disconnect() { return true; }
  bool send(const boost::string_ref buff, std::chrono::milliseconds timeout) { return true; }
  bool send(const void* data, size_t sz) { return true; }
  bool is_connected() { return true; }
  bool recv(std::string& buff, std::chrono::milliseconds timeout)
  {
    buff = data;
    data.clear();
    return true;
  }
  void set_ssl(epee::net_utils::ssl_options_t ssl_options) { }
  bool is_connected(bool *ssl = NULL) { return true; }
  uint64_t get_bytes_sent() const  { return 1; }
  uint64_t get_bytes_received() const { return 1; }

  void set_test_data(const std::string &s) { data = s; }

private:
  std::string data;
};

static epee::net_utils::http::http_simple_client_template<dummy_client> client;

BEGIN_INIT_SIMPLE_FUZZER()
END_INIT_SIMPLE_FUZZER()

BEGIN_SIMPLE_FUZZER()
  client.test(std::string((const char*)buf, len), std::chrono::milliseconds(1000));
END_SIMPLE_FUZZER()
