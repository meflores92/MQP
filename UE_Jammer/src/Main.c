/*
 =====================================================================================
 Name        : main.c
 Authors     : Colin Stevens, Maya Flores, and Devon Poisson
 Copyright   : 
 Description : Source file containing main function and most processing threads 
 ======================================================================================
 */

/*====================================== Defines ======================================*/
#define _GNU_SOURCE //Keep this at the top of the file! For CPU Assignment

//GUI Parameters
#define SNR_REFREASH_RATE 50 //RATE*2 == number of 5G frames
#define QPSK_PLOT_REFREASH_RATE 2 //RATE*2 == number of 5G frames
#define QPSK_PLOT_UPPER_BOUND 30
#define QPSK_PLOT_LOWER_BOUND -30
/*=====================================================================================*/


/*=================================== Include Files ===================================*/
//Real time includes
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <errno.h>
//Standard Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//MATLAB Code
#include "tmwtypes.h"
#include "MATLAB_Code/SyncFrame.h"
#include "MATLAB_Code/SyncFrameInit.h"
#include "MATLAB_Code/PDCCHCapture.h"
#include "MATLAB_Code/PDCCHFind.h"
#include "MATLAB_Code/getSNR.h"
#include "MATLAB_Code/rt_nonfinite.h"
#include "MATLAB_Code/operation_emxutil.h"
#include "MATLAB_Code/spectrogram.h"
#include "MATLAB_Code/SyncFrameRecover.h"
#include "MATLAB_Code/getPUSCH.h"
//MATLAB Timing functions
#include "timing/tic.h"
#include "timing/toc.h"
//GUI includes
#include "GUI/GUI.h"
//USRP includes
#include "trx/trx.h"
//Main includes 
#include "main.h"
#include "common.h"
/*=====================================================================================*/


/*================================= Global Variables ==================================*/
//Waveform Buffers
creal32_T rxWaveform[FRAME_BUFF_SIZE];
creal32_T data[1916];
creal32_T pdcch_buffer[delta];
float PDCCHIPhase[216];
float PDCCHQPhase[216];

//Shared Data information
unsigned short slot = 0;
unsigned short UERNTI = 65535;
int idx =0;
int offset = 0;
int head = 0;
int next = 0;
int tx_idx = 0;
unsigned int detectedRNTI;

// Freq. Resource Assignment Variables
signed char dcibits[39];
int k2 = 6; // k2 = 6 Slots
struct pusch_data pusch_fifo[40];
int PUSCHOffset = 0;


//Real Time globals
sem_t get_frame_offset;
sem_t decode_pdcch;
sem_t plotData;
sem_t plotSpec;
sem_t decode_dci, transmit, filter, detected;
pthread_t sync_id, pdcch_id, rx_id, GUI_id, plot_id, spec_id, pusch_id, tx_id, filter_id, detected_id;
pthread_attr_t syncAttr, rxAttr, pdcchAttr, GUIAttr, plotAttr, specAttr, puschAttr, txAttr, filterAttr, detectedAttr;

//Flags
int mode = 2;
int done = 1;
int syncronized = 0;
int done_decoding = 3;
int done_ploting = 1;
int done_gen_PUSCH= 1;
int plot_spec_flag = 0;

//Linked list of active UEs
struct ue* ueList = NULL;
/*=====================================================================================*/


/*================================= Program main ======================================*/
int main(void){

	initRealTime();
	create_args();
	init_usrp_rx();
	init_usrp_tx();

	pthread_create(&rx_id, &rxAttr, stream_rx_thread, (void *)&rx_id);
	pthread_create(&tx_id, &txAttr, stream_tx_thread, (void *)&tx_id);
	pthread_create(&pdcch_id, &pdcchAttr, pdcch_thread, (void *)&pdcch_id);
	pthread_create(&plot_id, &plotAttr, plot_PDCCH_thread, (void *)&plot_id);
	pthread_create(&spec_id, &specAttr, plot_Spectrogram_thread, (void *)&spec_id);
	pthread_create(&pusch_id, &puschAttr, pusch_thread, (void *)&pusch_id);
	pthread_create(&filter_id, &filterAttr, filter_thread, (void *)&filter_id);
	pthread_create(&detected_id, &detectedAttr, detected_UE_thread, (void *)&detected_id);
	pthread_create(&sync_id, &syncAttr, sync_thread, (void *)&sync_id);

	pthread_create(&GUI_id, &GUIAttr, GUI_thread, (void *)&GUI_id);

	pthread_join(rx_id,NULL);
	pthread_join(tx_id,NULL);
	pthread_join(pusch_id,NULL);
	pthread_join(sync_id,NULL);
	pthread_join(pdcch_id,NULL);
	pthread_join(GUI_id,NULL);
	pthread_join(plot_id,NULL);
	pthread_join(spec_id,NULL);
	pthread_join(filter_id,NULL);
	pthread_join(detected_id,NULL);

	pthread_attr_destroy(&filterAttr);
	pthread_attr_destroy(&syncAttr);
	pthread_attr_destroy(&pdcchAttr);
	pthread_attr_destroy(&rxAttr);
	pthread_attr_destroy(&GUIAttr);
	pthread_attr_destroy(&plotAttr);
	pthread_attr_destroy(&specAttr);
	pthread_attr_destroy(&puschAttr);
	pthread_attr_destroy(&txAttr);
	pthread_attr_destroy(&detectedAttr);

	sem_destroy(&get_frame_offset);
	sem_destroy(&decode_pdcch);
	sem_destroy(&plotData);
	sem_destroy(&plotSpec);
	sem_destroy(&decode_dci);
	sem_destroy(&transmit);
	sem_destroy(&filter);
	sem_destroy(&detected);

	terminate();
	
	return 0;
}
/*=====================================================================================*/


/*================================= Init Functions ====================================*/
//Initialize Real Time Parameters
void initRealTime(){
	//CPU set for low priority threads
	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);
	CPU_SET(0, &cpuset);

	//CPU set for USRP RX threads
	cpu_set_t RX_CPU;
	CPU_ZERO(&RX_CPU);
	CPU_SET(0, &RX_CPU);
	CPU_SET(1, &RX_CPU);
	CPU_SET(2, &RX_CPU);
	CPU_SET(3, &RX_CPU);


	//CPU set for USRP TX threads
	cpu_set_t TX_CPU;
	CPU_ZERO(&TX_CPU);
	CPU_SET(2, &TX_CPU);

	//CPU set for PUSCH threads	
	cpu_set_t PUSCH_CPU;
	CPU_ZERO(&PUSCH_CPU);
	CPU_SET(2, &PUSCH_CPU);

	//CPU set for PDCCH threads
	cpu_set_t PDCCH_CPU;
	CPU_ZERO(&PDCCH_CPU);
	CPU_SET(3, &PDCCH_CPU);

	struct sched_param param;

	param.sched_priority = DETECTED_PRIORITY;
	pthread_attr_init(&detectedAttr);
	pthread_attr_setschedpolicy(&detectedAttr, SCHEDULAR);
	pthread_attr_setinheritsched(&detectedAttr, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedparam(&detectedAttr, &param);
	pthread_attr_setscope(&detectedAttr, PTHREAD_SCOPE_SYSTEM);
	pthread_attr_setaffinity_np(&detectedAttr, sizeof(cpuset), &cpuset);

	param.sched_priority = FILTER_PRIORITY;
	pthread_attr_init(&filterAttr);
	pthread_attr_setschedpolicy(&filterAttr, SCHEDULAR);
	pthread_attr_setinheritsched(&filterAttr, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedparam(&filterAttr, &param);
	pthread_attr_setscope(&filterAttr, PTHREAD_SCOPE_SYSTEM);
	pthread_attr_setaffinity_np(&filterAttr, sizeof(cpuset), &cpuset);

	param.sched_priority = SYNC_PRIORITY;
	pthread_attr_init(&syncAttr);
	pthread_attr_setschedpolicy(&syncAttr, SCHEDULAR);
	pthread_attr_setinheritsched(&syncAttr, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedparam(&syncAttr, &param);
	pthread_attr_setscope(&syncAttr, PTHREAD_SCOPE_SYSTEM);
	pthread_attr_setaffinity_np(&syncAttr, sizeof(cpuset), &cpuset);

	param.sched_priority = GUI_PRIORITY;
	pthread_attr_init(&GUIAttr);
	pthread_attr_setschedpolicy(&GUIAttr, SCHEDULAR);
	pthread_attr_setinheritsched(&GUIAttr, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedparam(&GUIAttr, &param);
	pthread_attr_setscope(&GUIAttr, PTHREAD_SCOPE_SYSTEM);
	pthread_attr_setaffinity_np(&GUIAttr, sizeof(cpuset), &cpuset);

	param.sched_priority = PLOT_PRIORITY;
	pthread_attr_init(&plotAttr);
	pthread_attr_setschedpolicy(&plotAttr, SCHEDULAR);
	pthread_attr_setinheritsched(&plotAttr, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedparam(&plotAttr, &param);
	pthread_attr_setscope(&plotAttr, PTHREAD_SCOPE_SYSTEM);
	pthread_attr_setaffinity_np(&plotAttr, sizeof(cpuset), &cpuset);

	param.sched_priority = PLOT_PRIORITY;
	pthread_attr_init(&specAttr);
	pthread_attr_setschedpolicy(&specAttr, SCHEDULAR);
	pthread_attr_setinheritsched(&specAttr, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedparam(&specAttr, &param);
	pthread_attr_setscope(&specAttr, PTHREAD_SCOPE_SYSTEM);
	pthread_attr_setaffinity_np(&specAttr, sizeof(cpuset), &cpuset);

	param.sched_priority = USRP_RX_PRIORITY;
	pthread_attr_init(&rxAttr);
	pthread_attr_setschedpolicy(&rxAttr, SCHEDULAR);
	pthread_attr_setinheritsched(&rxAttr, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedparam(&rxAttr, &param);
	pthread_attr_setscope(&rxAttr, PTHREAD_SCOPE_SYSTEM);
	pthread_attr_setaffinity_np(&rxAttr, sizeof(RX_CPU), &RX_CPU);

	param.sched_priority = USRP_TX_PRIORITY;
	pthread_attr_init(&txAttr);
	pthread_attr_setschedpolicy(&txAttr, SCHEDULAR);
	pthread_attr_setinheritsched(&txAttr, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedparam(&txAttr, &param);
	pthread_attr_setscope(&txAttr, PTHREAD_SCOPE_SYSTEM);
	pthread_attr_setaffinity_np(&txAttr, sizeof(TX_CPU), &TX_CPU);

	param.sched_priority = PUSCH_PRIORITY;
	pthread_attr_init(&puschAttr);
	pthread_attr_setschedpolicy(&puschAttr, SCHEDULAR);
	pthread_attr_setinheritsched(&puschAttr, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedparam(&puschAttr, &param);
	pthread_attr_setscope(&puschAttr, PTHREAD_SCOPE_SYSTEM);
	pthread_attr_setaffinity_np(&puschAttr, sizeof(PUSCH_CPU), &PUSCH_CPU);

	param.sched_priority = PDCCH_PRIORITY;
	pthread_attr_init(&pdcchAttr);
	pthread_attr_setschedpolicy(&pdcchAttr, SCHEDULAR);
	pthread_attr_setinheritsched(&pdcchAttr, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedparam(&pdcchAttr, &param);
	pthread_attr_setscope(&pdcchAttr, PTHREAD_SCOPE_SYSTEM);
	pthread_attr_setaffinity_np(&pdcchAttr, sizeof(PDCCH_CPU), &PDCCH_CPU);

	sem_init(&get_frame_offset, 0,0);
	sem_init(&decode_pdcch, 0,0);
	sem_init(&plotData, 0,0);
	sem_init(&plotSpec, 0,0);
	sem_init(&transmit, 0, 0);
	sem_init(&decode_dci, 0,0);
	sem_init(&filter, 0,0);
	sem_init(&detected, 0,0);

}
/*=====================================================================================*/


/*============================= Primary Jammer Threads ================================*/
//Thread for Frame sync
void* sync_thread(){
	//GUI Variables
    GdkColor red,green;
    gdk_color_parse("red", &red);
    gdk_color_parse("green", &green);
    int status= 0; //Current sync status
    int frames = 0;
    float SNR;
	float SNR_avg[100];
	int index = 0;
	char SNRString[30];
	char PDCCHLabel[50];

	//Temp Buffers
	creal32_T syncWave[6584];
	creal32_T recoverWave[FRAME_BUFF_SIZE/2];
	creal32_T initWave[FRAME_BUFF_SIZE];

	//Indexing variables
	int x;
	int new_offset;


	//Wait until a whole frame has been received then find the initial offset
	sem_wait(&get_frame_offset);
	for(x = 0; x <FRAME_BUFF_SIZE; x++){
		initWave[x] = rxWaveform[x];
	}
	offset= SyncFrameInit(initWave);
	syncronized = 1;
    gtk_widget_modify_bg(syncStatusLamp, GTK_STATE_NORMAL, &green);
    status = 1;


    //After receiving the first 6584 samples of a frame (symbols 0-3) resync with frame
	while(done != 0){
		sem_wait(&get_frame_offset);
		if(done != 0){
			if(syncronized ==1){
				for(x=0;x<6584;x++){
					syncWave[x] = rxWaveform[(offset+x+FRAME_BUFF_SIZE)%FRAME_BUFF_SIZE];
				}
				new_offset = SyncFrame(syncWave);
				offset += new_offset;
			}else{
				for(x=0;x<FRAME_BUFF_SIZE;x++){
					//recoverWave[x] = rxWaveform[(offset+x+FRAME_BUFF_SIZE-230400)%FRAME_BUFF_SIZE];
					initWave[x] = rxWaveform[x];
				}
				//new_offset = SyncFrameRecover(recoverWave) - 230400;
				offset= SyncFrameInit(initWave);
				//offset += new_offset;
				syncronized = 3;
			}

			//Update GUI Sync Status
			if(abs(new_offset)> OUT_OF_SYNC && status == 1){
				gtk_widget_modify_bg(syncStatusLamp, GTK_STATE_NORMAL, &red);
				status = 0;
				get_sync = 3;
			}
			else if (status == 0 && abs(new_offset) < IN_SYNC){
				status = 1;
				gtk_widget_modify_bg(syncStatusLamp, GTK_STATE_NORMAL, &green);
			}


			//Update PDCCH Percent GUI
			if(updatePDCCHPercentFlag == 1){
				sprintf(PDCCHLabel, "Percentage of Decoded PDCCH Candidates: %.2f%%", PDCCHPercent);
				gtk_label_set_text(GTK_LABEL(PDCCHPerformanceLabel), PDCCHLabel);
				updatePDCCHPercentFlag = 0;
			}

			//Update SNR measurement
			if(status == 1){
				frames++;
				if(frames > SNR_REFREASH_RATE){
					frames = 0;
					SNR = getSNR(syncWave);
					SNR_avg[index%100] = SNR;
					index++;
					sprintf(SNRString, "Estimated SNR: %.2fdB", SNR);
					gtk_label_set_text(GTK_LABEL(SNRLabel), SNRString);
				}
			}
		}
	}
	float total = 0;
		
	for(index = 0; index < 100; index++){
		total += SNR_avg[index];
	}
	printf("SNR: %f", total/100);
	return NULL;
}


//Thread for decoding PDCCH
void* pdcch_thread(){
	//Local Buffers for storing QPSK Constellation Info
	float IPhase[216];
	float QPhase[216];

	//PDCCH Inputs and Outputs
	unsigned short dmrsRNTI = 0;
	unsigned short scramblingRNTI = 0;
	unsigned int newRNTI;
    double isValid = 1;

    //Misc variables
	int index;
	int prevSlot = 0;
	int numFrames =0;
	unsigned short PDCCHSlot = 0;

	done_decoding = 1;
	while(done!=0){
		done_decoding = 1;
		sem_wait(&decode_pdcch);
		if(done !=0){
			done_decoding = 0;
			for(index = 0; index<delta; index++){
				pdcch_buffer[index] = rxWaveform[(index+offset+slot_lut[slot])%FRAME_BUFF_SIZE];
			}
			PDCCHSlot = slot%20;
			if(mode==0){ //Find UE Mode
				PDCCHFind(pdcch_buffer, PDCCHSlot, dcibits, &newRNTI,
						&isValid, IPhase, QPhase);
				if(isValid ==1){
					detectedRNTI = newRNTI;
					sem_post(&detected);
				}
				if(slot < prevSlot){ //Filter UEs every frame
					sem_post(&filter);
				}
				prevSlot = slot;
			}
			else if(mode == 1){ //Active Jam mode
				PDCCHCapture(pdcch_buffer, dmrsRNTI, scramblingRNTI, UERNTI, PDCCHSlot,
						dcibits, &isValid, IPhase, QPhase);

				if (isValid == 1 && UERNTI != 65535){
					if(done_gen_PUSCH == 1){
						pusch_fifo[next].slot = slot;
						sem_post(&decode_dci);
					}
					else{
						printf("Missed PUSCH at slot %i\n", slot);
					}

				}
			}
			if(mode != 2 && plotFlag && done_ploting == 1 && isValid == 1){
				numFrames++;
				if(numFrames>=QPSK_PLOT_REFREASH_RATE){
					for(index = 0; index< 216; index++){
						PDCCHIPhase[index] = IPhase[index];
						PDCCHQPhase[index] = QPhase[index];
					}
					numFrames=0;
					sem_post(&plotData);
				}
			}
		}
	}
	return NULL;
}


//Thread for decoding DCI and generating jamming signal 
void *pusch_thread(){
	int i = 0;
	int j = 0;
	int x = 0;
	unsigned short Lrb = 0;
	unsigned short RBstart = 0;
	int Nrb = 48;
	int RIV_SIZE = 11;
	int RIV_count = 0;
	int RIV = 0;
	int index = 0;
	creal32_T JamWaveform[1536];

	while (done != 0){
		RIV_count = 0;
		done_gen_PUSCH = 1;

		sem_wait(&decode_dci);
		done_gen_PUSCH = 0;
		if(done!=0){
			// Check if format is 0_0
			if (dcibits[0] == 0){

				// Find RIV
				RIV = 0;
				for (x = 1; x <= RIV_SIZE; x++){
					if (dcibits[x] == 1){
						RIV += (int)pow(2.0, (double)(RIV_SIZE - x));
					}
				}
				
				done_gen_PUSCH =1;

				// Decode RIV to Obtain Lrb and RBstart
				for (i = 0; i < Nrb; i++){
					for (j = 0; j < (Nrb - i); j++){
						if ((i - 1) <= (Nrb / 2)){
							if (RIV == Nrb * (i - 1) + j){
								Lrb = i;
								RBstart = j;
								RIV_count++;
							}
						}
						else{
							if (RIV == Nrb * (Nrb - i + 1) + (Nrb - 1 - j)){
								Lrb = i;
								RBstart = j;
								RIV_count++;
							}
						}
					}
				}

				if(RIV_count == 1){
					//Calculate Starting Subcarrier from RBstart
					getPUSCH(RBstart+27, Lrb, JamWaveform);
					int convertIndex = 0;
					for(index = 0; index < 1536*2; index+=2){
						pusch_fifo[next].waveform[index] = JamWaveform[convertIndex].re;
						pusch_fifo[next].waveform[index+1] = JamWaveform[convertIndex].im;
						convertIndex++;
					}
					next = (next + 1) % 40;
				}
				else{
					printf("Skipped PUSCH... Multiple or False RIV\n");
				}
			}
		}
	}
	return NULL;
}


// Thread to filter noise from UE detection
void* filter_thread(){
	while(done != 0){
		sem_wait(&filter);
		if(done != 0){
			struct ue** head = &ueList;
			struct ue* currentUE = *head;
			while(currentUE != NULL){
				if(currentUE->numFrames>100){
					float ratio = ((float)currentUE->numOccur/currentUE->numFrames);
					currentUE->numFrames = 10;
					currentUE->numOccur = (int)(ratio*10);
				}
				currentUE->numFrames++;

				if(((float)currentUE->numOccur/currentUE->numFrames) < REMOVE_UE){
					currentUE->isActive = 0;
					if(currentUE->inlist == 1){
						updateUEList();
					}
					if(currentUE->prev == NULL){
						currentUE = NULL;
						continue;
					}
					currentUE->prev->next = currentUE->next;
					if(currentUE->next != NULL){
						struct ue *temp = currentUE->prev;
						currentUE = currentUE->next;
						free(currentUE->prev);
						currentUE->prev = temp;
						continue;
					}
				}

				if(((float)currentUE->numOccur/currentUE->numFrames) > ACTIVE_UE && currentUE->isActive == 0 && currentUE->numFrames > 50){
					currentUE->isActive = 1;
					updateUEList();
				}
				currentUE = currentUE->next;
			}	
		}
	}
	return NULL;
}


// Handle when a UE has been detected
void* detected_UE_thread(){
	
	while(done != 0){
		if(done != 0){
			sem_wait(&detected);
			struct ue** head = &ueList;
			struct ue* currentUE = *head;
			int foundUE = 0;
			if(currentUE == NULL){
				currentUE = (struct ue*)malloc(sizeof(struct ue));
				currentUE->RNTI = detectedRNTI;
				currentUE->next = NULL;
				currentUE->prev = NULL;
				currentUE->numOccur = 1;
				currentUE->numFrames = 1;
				currentUE->next = NULL;
				currentUE->isActive = 0;
				currentUE->inlist = 0;
				*head = currentUE;

			}else{

				while(currentUE->next != NULL){
					if(currentUE->RNTI == detectedRNTI){
						currentUE->numOccur += 1;
						foundUE = 1;
						break;
					}
					currentUE = currentUE->next;
				}
				if(foundUE == 0 && currentUE->RNTI == detectedRNTI){
					currentUE->numOccur += 1;
					foundUE = 1;
				}
				if(foundUE == 0){
					struct ue *newUE = (struct ue*)malloc(sizeof(struct ue));
					newUE->RNTI = detectedRNTI;
					newUE->next = NULL;
					newUE->prev = currentUE;
					newUE->numOccur = 1;
					newUE->numFrames = 1;
					newUE->isActive = 0;
					newUE->inlist = 0;
					currentUE->next = newUE;
				}
			}
		}
	}
	return NULL;
}
/*=====================================================================================*/


/*======================== GUI Threads and Supporting Functions =======================*/
//GUI thread
void* GUI_thread(){
	initGUI();
	return NULL;
}


//Update GUI list of Active UEs
void updateUEList(){
	char UEString[2000];
	char addUE1[200];
	int index = 0;
	int UEIndex = 0;
	struct ue *currentUE = ueList;

	sprintf(addUE1, "List of UE on Network\n");
    while(addUE1[UEIndex] != '\0' && UEIndex < 100){
    	UEString[index] = addUE1[UEIndex];
    	UEIndex++;
    	index++;
    }
    UEIndex = 0;

	while(currentUE != NULL){
		if(currentUE->isActive == 1){
			char addUE[200];
			if(currentUE->RNTI == 65535) sprintf(addUE, "Network RNTI: %i\n", currentUE->RNTI);
			else sprintf(addUE, "UE RNTI: %i\n", currentUE->RNTI);
			while(addUE[UEIndex] != '\0' && UEIndex < 100){
				UEString[index] = addUE[UEIndex];
				UEIndex++;
				index++;
			}
			UEIndex = 0;
			currentUE->inlist = 1;
		}
		currentUE = currentUE->next;
	}

	UEString[index] = '\0';
	gtk_label_set_text(GTK_LABEL(listItem), UEString);
	gtk_widget_show(listItem);
}


//Plot PDDCH thread
void* plot_PDCCH_thread(){
	int isFirst =1;
	int index;
	FILE* gnuplot_pipe;

	while(done != 0){
		done_ploting = 1;
		sem_wait(&plotData);
		done_ploting = 0;
		if(done != 0){
			FILE* dataFile = fopen("PDCCHData.txt", "w");
			for(index = 0; index<216; index++){
				fprintf(dataFile, "%f   %f\n", PDCCHIPhase[index], PDCCHQPhase[index]);
			}
			fclose(dataFile);
			if(isFirst==1){
				gnuplot_pipe = popen("gnuplot", "w");
				fprintf(gnuplot_pipe, "set title '%s' font ',25'\n", "PDCCH Constellation");
				fprintf(gnuplot_pipe, "plot \"PDCCHData.txt\"\n");
				fprintf(gnuplot_pipe, "set xrange [%i:%i] \n", QPSK_PLOT_LOWER_BOUND, QPSK_PLOT_UPPER_BOUND);
				fprintf(gnuplot_pipe, "set yrange [%i:%i] \n",QPSK_PLOT_LOWER_BOUND,QPSK_PLOT_UPPER_BOUND);
				fprintf(gnuplot_pipe, "set xlabel \"In-Phase\" font \",20\"\n");
				fprintf(gnuplot_pipe, "set ylabel \"Quadrature\" font \",20\"\n");
				fprintf(gnuplot_pipe, "set xzeroaxis\n");
				fprintf(gnuplot_pipe, "set yzeroaxis\n");

				fprintf(gnuplot_pipe, "replot\n");

				isFirst = 0;
			}
			else{
				fprintf(gnuplot_pipe, "replot\n");
			}
			fflush(gnuplot_pipe);
			//plot_PDCCH();
		}else{

		}
	}
	if(isFirst != 1){
		fprintf(gnuplot_pipe, "exit gnuplot");
		fclose(gnuplot_pipe);
	}

	return NULL;
}


// plot Spectrogram thread
void* plot_Spectrogram_thread(){
	//MATLAB spectrum function inputs and outputs
	creal32_T rxWaveformLocal[FRAME_BUFF_SIZE];
	float tmp_data[1536];
	float fv[600];
	int tmp_size;

	//Misc variables
	int localOffset;
	int isFirst =1;
	int row,col,index;
	FILE* gnuplot_pipe;
	int localIdx;
	const char setPalette[] = "set palette defined ( 0 '#000090',\
                      1 '#000fff',\
                      2 '#0090ff',\
                      3 '#0fffee',\
                      4 '#90ff70',\
                      5 '#ffee00',\
                      6 '#ff7000',\
                      7 '#ee0000',\
                      8 '#7f0000')\n";
	const char setMacro[] = "set macros\nMATLAB = \"defined (0  0.0 0.0 0.5, \
                   1  0.0 0.0 1.0, \
                   2  0.0 0.5 1.0, \
                   3  0.0 1.0 1.0, \
                   4  0.5 1.0 0.5, \
                   5  1.0 1.0 0.0, \
                   6  1.0 0.5 0.0, \
                   7  1.0 0.0 0.0, \
                   8  0.5 0.0 0.0 )\"\n";


	while(done != 0){
		sem_wait(&plotSpec);
		if(done != 0){
			plot_spec_flag = 0;
			localIdx = idx;
			while(abs(localIdx-idx) < (1916*50)){
				usleep(50);
			}

			emxArray_creal32_T *r;
			emxArray_real32_T *r1;
			emxInit_creal32_T(&r, 2);
			emxInit_real32_T(&r1, 2);
			localOffset = offset;
			FILE* specWaveFile = fopen("SpecWave.txt", "w");
			for(index = 0; index< FRAME_BUFF_SIZE; index++){
				rxWaveformLocal[index] = rxWaveform[(index+localOffset+FRAME_BUFF_SIZE)%FRAME_BUFF_SIZE];
				//rxWaveformLocal[index] = rxWaveform[index];
			}
			for(index = 0; index < FRAME_BUFF_SIZE; index++){
				fprintf(specWaveFile, "%f\n%f\n", rxWaveformLocal[index].re, rxWaveformLocal[index].im);
			}

			//fwrite(rxWaveformLocal, 4, )
			fclose(specWaveFile);
			spectrogram(rxWaveformLocal, r, tmp_data,
			              &tmp_size, fv, r1);
			emxFree_creal32_T(&r);

			FILE* specFile = fopen("SpecData.txt", "w");
			for(col = 0; col<1536; col++){
				for(row= 0;row<600;row++){
					fprintf(specFile, "%f ", 10*log10(r1->data[col+(row*1536)]));
				}
				fprintf(specFile, "\n");
			}
			fclose(specFile);
			if(isFirst==1){
				gnuplot_pipe = popen("gnuplot -persist", "w");
				isFirst = 0;
			}
			fprintf(gnuplot_pipe, "set title '%s' font ',25'\n", "Spectrogram");
			fprintf(gnuplot_pipe, setPalette);
			fprintf(gnuplot_pipe, setMacro);
			fprintf(gnuplot_pipe, "set palette @MATLAB\n");
			fprintf(gnuplot_pipe, "set xlabel \"OFDM Symbols\" font \",20\"\n");
			fprintf(gnuplot_pipe, "set ylabel \"Baseband Frequency (MHz)\" font \",20\"\n");

			fprintf(gnuplot_pipe, "set yrange [-23:23]\n");
			fprintf(gnuplot_pipe, "set xrange [0:560]\n");
			fprintf(gnuplot_pipe, "plot \"SpecData.txt\" u ($1*560/600):(($2*30e3)-(23.04e6))/1e6:3 matrix with image \n");
			emxFree_real32_T(&r1);
			fflush(gnuplot_pipe);
		}
	}

	if(isFirst != 1){
		fprintf(gnuplot_pipe, "exit gnuplot");
		fclose(gnuplot_pipe);
	}
	return NULL;
}
/*=====================================================================================*/


