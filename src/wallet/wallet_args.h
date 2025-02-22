// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
//
#include <boost/optional/optional.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/positional_options.hpp>
#include <boost/program_options/variables_map.hpp>

#include "common/command_line.h"

namespace wallet_args
{
  command_line::arg_descriptor<std::string> arg_generate_from_json();
  command_line::arg_descriptor<std::string> arg_wallet_file();
  command_line::arg_descriptor<std::string> arg_password_file();

  const char* tr(const char* str);

  /*! Processes command line arguments (`argc` and `argv`) using `desc_params`
  and `positional_options`, while adding parameters for log files and
  concurrency. Log file and concurrency arguments are handled, along with basic
  global init for the wallet process.

  \return
    pair.first: The list of parsed options, iff there are no errors.
    pair.second: Should the execution terminate succesfully without actually launching the application
  */
  std::pair<boost::optional<boost::program_options::variables_map>, bool> main(
    int argc, char** argv,
    const char* const usage,
    const char* const notice,
    boost::program_options::options_description desc_params,
    const boost::program_options::positional_options_description& positional_options,
    const std::function<void(const std::string&, bool)> &print,
    const char *default_log_name, bool log_to_console = false);
}
