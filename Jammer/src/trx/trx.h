/*
 * rx.h
 *
 *  Created on: Jan 18, 2022
 *      Author: tornado
 */

#ifndef TRX_RXH_TXT_
#define TRX_RXH_TXT_

#include "../tmwtypes.h"
#include <semaphore.h>
#include <pthread.h>
#include "../GUI/GUI.h"

//Define Constants
#define mboard (size_t)0
#define center_freq (double)3619200000
#define FRAME_BUFF_SIZE 921600//921600   460800
#define NUM_TX_SAMPS 1916*13
#define delta 1668

struct pusch_data{
	unsigned short slot;
	float waveform[1536*2];
};

//Function Declaration
int create_args();
int init_usrp_rx();
void init_usrp_tx();
void *stream_rx_thread();
void *stream_tx_thread();
int terminate();

//Extern Flags and Indexing Variables
extern int done;
extern int idx;
extern int tx_idx;
extern int done_decoding;
extern int mode;
extern int syncronized;
extern int get_sync;
extern int offset;
extern int send_symbol_length;
extern int 	plot_spec_flag;
extern int PUSCHOffset;
extern float PDCCHPercent;
extern int updatePDCCHPercentFlag;
extern const int slot_lut[40];


//Extern Buffers
extern creal32_T rxWaveform[FRAME_BUFF_SIZE];
extern creal32_T pdcch_buffer[delta];
extern float sine_data[1274][49816];
extern float tx_sine_buff[49816];
extern float jamming_signals[5565][49816];
extern int Lrb_idx[106];

//Extern Semaphores
extern sem_t get_frame_offset;
extern sem_t decode_pdcch;
extern sem_t transmit;
extern sem_t decode_dci;
extern sem_t plotData;
extern sem_t plotSpec;
extern sem_t filter;
extern sem_t detected;

//Extern PUSCH Variables
extern unsigned short slot;
extern int k2;
extern struct pusch_data pusch_fifo[40];
extern int head;
extern int next;
extern int target_slot;

#endif /* TRX_RXH_TXT_ */
