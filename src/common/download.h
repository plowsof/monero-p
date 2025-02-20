// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 

#pragma once 

#include <string>

namespace tools
{
  struct download_thread_control;
  typedef std::shared_ptr<download_thread_control> download_async_handle;

  bool download(const std::string &path, const std::string &url, std::function<bool(const std::string&, const std::string&, size_t, ssize_t)> progress = NULL);
  download_async_handle download_async(const std::string &path, const std::string &url, std::function<void(const std::string&, const std::string&, bool)> result, std::function<bool(const std::string&, const std::string&, size_t, ssize_t)> progress = NULL);
  bool download_error(const download_async_handle &h);
  bool download_finished(const download_async_handle &h);
  bool download_wait(const download_async_handle &h);
  bool download_cancel(const download_async_handle &h);
}
