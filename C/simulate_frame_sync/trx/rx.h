/*
 * rx.h
 *
 *  Created on: Jan 18, 2022
 *      Author: tornado
 */

#ifndef TRX_RX_H_
#define TRX_RX_H_

#include "../tmwtypes.h"
#include <semaphore.h>

#define mboard (size_t)0
#define center_freq (double)3619200000
#define FRAME_BUFF_SIZE 921600//921600   460800


//Function Declaration
int create_args();
int init_usrp();
int *stream_rx_thread();
int terminate();

extern int done;
extern int idx;
extern creal32_T rxWaveform[FRAME_BUFF_SIZE];
extern sem_t get_frame_offset;
extern int syncronized;
extern int offset;

#endif /* TRX_RX_H_ */
