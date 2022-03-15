#ifndef MAIN_H_
#define MAIN_H_

//Thread priority
#define USRP_RX_PRIORITY 99
#define USRP_TX_PRIORITY 98
#define PUSCH_PRIORITY 98
#define PDCCH_PRIORITY 99
#define PLOT_PRIORITY 1
#define GUI_PRIORITY 68
#define SYNC_PRIORITY 68
#define FILTER_PRIORITY 70
#define DETECTED_PRIORITY 70

//Real time schedular to use
#define SCHEDULAR SCHED_RR

//Threshold for filtering detected UE
#define REMOVE_UE .001
#define ACTIVE_UE .01

//Sync status boundaries
#define OUT_OF_SYNC 500
#define IN_SYNC 10

//Function Definitions
void* sync_thread();
void* pdcch_thread();
void* detected_UE_thread();
void updateUEList();
void* filter_thread();
void* GUI_thread();
void* plot_PDCCH_thread();
void* plot_Spectrogram_thread();
void initRealTime();
void *pusch_thread();

//Structure for storing information on detected UEs
struct ue{
	unsigned int RNTI;
	unsigned int numOccur;
	unsigned int numFrames;
	unsigned int isActive;
	struct ue *next;
	struct ue *prev;
	int inlist;
};

//Extern variables
extern const int slot_lut[40];

#endif
