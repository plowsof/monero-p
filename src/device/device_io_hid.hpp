// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 
//

#if defined(HAVE_HIDAPI) 

#include <boost/optional/optional.hpp>
#include <hidapi/hidapi.h>
#include "device_io.hpp"

#pragma once

namespace hw {
  namespace io {



    /** HID class base. Commands are formated as follow:
     *
     * |----------------------------------------------------------|
     * |  2 bytes  |  1 byte  |  2 bytes  | 2 bytes  |  len bytes |
     * |-----------|----------|-----------|----------|------------|
     * |  channel  |    tag   |  sequence |   len    |  payload   |
     * |----------------------------------------------------------|
     */


    struct hid_conn_params {
      unsigned int vid; 
      unsigned int pid; 
      int interface_number;
      unsigned short usage_page;
    };
    

    class device_io_hid: device_io {
      

    private:
     

      unsigned short channel;
      unsigned char  tag;
      unsigned int   packet_size; 
      unsigned int   timeout;

      unsigned int   usb_vid;
      unsigned int   usb_pid;
      hid_device     *usb_device;

      void io_hid_log(int read, unsigned char* buf, int buf_len);
      void io_hid_init();
      void io_hid_exit() ;
      void io_hid_open(int vid, int pid,  int mode);
      void io_hid_close (void);

      unsigned int wrapCommand(const unsigned char *command, size_t command_len, unsigned char *out, size_t out_len);
      unsigned int unwrapReponse(const unsigned char *data, size_t data_len, unsigned char *out, size_t out_len);
 
      hid_device_info *find_device(hid_device_info *devices_list, boost::optional<int> interface_number, boost::optional<unsigned short> usage_page);
 
    public:
      bool hid_verbose = false;

      static const unsigned short DEFAULT_CHANNEL     = 0x0001;
      static const unsigned char  DEFAULT_TAG         = 0x01;
      static const unsigned int   DEFAULT_PACKET_SIZE = 64;
      static const unsigned int   DEFAULT_TIMEOUT     = 120000;

      device_io_hid(unsigned short channel, unsigned char tag, unsigned int packet_zize, unsigned int timeout);
      device_io_hid();
      ~device_io_hid() {};

      void init();  
      void connect(void *params);
      void connect(const std::vector<hid_conn_params> &conn);
      hid_device  *connect(unsigned int vid, unsigned  int pid, boost::optional<int> interface_number, boost::optional<unsigned short> usage_page);
      bool connected() const;
      int  exchange(unsigned char *command, unsigned int cmd_len, unsigned char *response, unsigned int max_resp_len, bool user_input);
      void disconnect();
      void release();
    };
  };
};

#endif //#if defined(HAVE_HIDAPI) 
