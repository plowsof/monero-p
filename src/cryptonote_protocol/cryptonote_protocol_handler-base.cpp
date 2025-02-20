/// @file
/// @author rfree (current maintainer in monero.cc project)
/// @brief This is the place to implement our handlers for protocol network actions, e.g. for ratelimit for download-requests

// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 

#include <string>
#include <vector>

#include <memory>

#include "syncobj.h"

#include "net/net_utils_base.h" 
#include "misc_log_ex.h" 
#include <boost/chrono.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp> 
#include "misc_language.h"
#include <algorithm>


#include "cryptonote_protocol_handler.h"
#include "net/network_throttle.hpp"

#include "cryptonote_core/cryptonote_core.h" // e.g. for the send_stop_signal()

#undef MONERO_DEFAULT_LOG_CATEGORY
#define MONERO_DEFAULT_LOG_CATEGORY "net.cn"

// ################################################################################################
// ################################################################################################
// the "header part". Not separated out for .hpp because point of this modification is 
// to rebuild just 1 translation unit while working on this code.
// (But maybe common parts will be separated out later though - if needed)
// ################################################################################################
// ################################################################################################

namespace cryptonote {

class cryptonote_protocol_handler_base_pimpl { // placeholer if needed
	public:

};

} // namespace

// ################################################################################################
// ################################################################################################
// ################################################################################################
// ################################################################################################

namespace cryptonote { 

double cryptonote_protocol_handler_base::estimate_one_block_size() noexcept { // for estimating size of blocks to downloa
 const double size_min = 500; // XXX 500
 //const int history_len = 20; // how many blocks to average over

 double avg=0;
 try {
 avg = get_avg_block_size(/*history_len*/);
 } catch (...) { }
 avg = std::max( size_min , avg);
 return avg;
}

cryptonote_protocol_handler_base::cryptonote_protocol_handler_base() {
}

cryptonote_protocol_handler_base::~cryptonote_protocol_handler_base() {
}

void cryptonote_protocol_handler_base::handler_request_blocks_history(std::list<crypto::hash>& ids) {
}

void cryptonote_protocol_handler_base::handler_response_blocks_now(size_t packet_size) {
	using namespace epee::net_utils;
	double delay=0; // will be calculated
	MDEBUG("Packet size: " << packet_size);
	do
	{ // rate limiting
		//XXX 
		/*if (::cryptonote::core::get_is_stopping()) { 
			MDEBUG("We are stopping - so abort sleep");
			return;
		}*/
		/*if (m_was_shutdown) { 
			MDEBUG("m_was_shutdown - so abort sleep");
			return;
		}*/

		{ 
	  	CRITICAL_REGION_LOCAL(	network_throttle_manager::m_lock_get_global_throttle_out );
			delay = network_throttle_manager::get_global_throttle_out().get_sleep_time_after_tick( packet_size );
		}

		
		delay *= 0.50;
		//delay = 0; // XXX
		if (delay > 0) {
			//delay += rand2*0.1;
            		long int ms = (long int)(delay * 1000);
			MDEBUG("Sleeping for " << ms << " ms before packet_size="<<packet_size); // XXX debug sleep
			boost::this_thread::sleep(boost::posix_time::milliseconds( ms ) ); // TODO randomize sleeps
		}
	} while(delay > 0);

// XXX LATER XXX
	{
	  CRITICAL_REGION_LOCAL(	network_throttle_manager::m_lock_get_global_throttle_out );
		network_throttle_manager::get_global_throttle_out().handle_trafic_tcp( packet_size ); // increase counter - global
		//epee::critical_region_t<decltype(m_throttle_global_lock)> guard(m_throttle_global_lock); // *** critical *** 
		//m_throttle_global.m_out.handle_trafic_tcp( packet_size ); // increase counter - global
	}
}

} // namespace


