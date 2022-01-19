/*
 * ref_lock.h
 *
 *  Created on: Jan 17, 2022
 *      Author: tornado
 */

#ifndef CPP_REF_LOCK_H_
#define CPP_REF_LOCK_H_

extern "C" #include <uhd/usrp/multi_usrp.hpp>

#ifdef __cplusplus
	extern "C" {
#endif

	void check_ref_lock(uhd::usrp::multi_usrp::sptr usrp, size_t mboard);

#ifdef __cplusplus
	}
#endif

#endif /* CPP_REF_LOCK_H_ */
