// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 

#include <libusb.h>

#define UNUSED(expr) (void)(expr)

int main(int argc, char *argv[]) {
  libusb_device **devs;
  libusb_context *ctx = NULL;

  int r = libusb_init(&ctx); UNUSED(r);
  ssize_t cnt = libusb_get_device_list(ctx, &devs); UNUSED(cnt);

  struct libusb_device_descriptor desc;
  r = libusb_get_device_descriptor(devs[0], &desc); UNUSED(r);
  uint8_t bus_id = libusb_get_bus_number(devs[0]); UNUSED(bus_id);
  uint8_t addr = libusb_get_device_address(devs[0]); UNUSED(addr);

  uint8_t tmp_path[16];
  r = libusb_get_port_numbers(devs[0], tmp_path, sizeof(tmp_path));
  UNUSED(r);
  UNUSED(tmp_path);

  libusb_free_device_list(devs, 1);
  libusb_exit(ctx);
}
