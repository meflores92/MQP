#ifndef TRX_RXH_TXT_
#define TRX_RXH_TXT_

#include "../tmwtypes.h"
#include <semaphore.h>
#include <pthread.h>
#include "../GUI/GUI.h"

//Define USRP constants
#define mboard (size_t)0
#define center_freq (double)3619200000

//Define constants used in RX thread
#define FRAME_BUFF_SIZE 921600//921600   460800
#define NUM_TX_SAMPS 1916*13
#define delta 1668

//Function Declaration
void create_args();
void init_usrp_rx();
void init_usrp_tx();
void *stream_rx_thread();
void *stream_tx_thread();
int terminate();

struct pusch_data{
	unsigned short slot;
	float waveform[1536*2];
};

//Extern Flags and Indexing Variables
extern int done;
extern int idx;
extern int tx_idx;
extern int done_decoding;
extern int mode;
extern int syncronized;
extern int get_sync;
extern int offset;
extern int 	plot_spec_flag;
extern int PUSCHOffset;
extern float PDCCHPercent;
extern int updatePDCCHPercentFlag;

//Extern Buffers
extern creal32_T rxWaveform[FRAME_BUFF_SIZE];
extern creal32_T pdcch_buffer[delta];

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

#endif /* TRX_RXH_TXT_ */
