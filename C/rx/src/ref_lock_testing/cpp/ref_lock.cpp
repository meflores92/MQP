/*
 * ref_lock.cpp
 *
 *  Created on: Jan 17, 2022
 *      Author: tornado
 */
#include "ref_lock.h"

#include <uhd/usrp/multi_usrp.hpp>
#include <uhd/utils/safe_main.hpp>
#include <uhd/utils/thread.hpp>
#include <boost/format.hpp>
#include <boost/program_options.hpp>
#include <chrono>
#include <iostream>
#include <thread>

extern "C" void check_ref_lock(uhd::usrp::multi_usrp::sptr usrp, size_t mboard){
	std::vector<std::string> sensor_names = usrp->get_mboard_sensor_names(mboard);
	if (std::find(sensor_names.begin(), sensor_names.end(), "ref_locked")
			!= sensor_names.end()) {
		std::cout << "Waiting for reference lock..." << std::flush;
		bool ref_locked = false;
		for (int i = 0; i < 30 and not ref_locked; i++) {
			ref_locked = usrp->get_mboard_sensor("ref_locked", mboard).to_bool();
			if (not ref_locked) {
				std::cout << "." << std::flush;
				//std::this_thread::sleep_for(std::chrono::seconds(1));
			}
		}
		if (ref_locked) {
			std::cout << "LOCKED" << std::endl;
		} else {
			std::cout << "FAILED" << std::endl;
			std::cout << "Failed to lock to GPSDO 10 MHz Reference. Exiting."
					<< std::endl;
			exit(EXIT_FAILURE);
		}
	} else {
		std::cout << boost::format(
				"ref_locked sensor not present on this board.\n");
	}
}

