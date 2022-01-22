/*
 * Main.c
 *
 *  Created on: Jan 16, 2022
 *      Author: tornado
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "simWaveform/loadWaveForm.h"
#include "simWaveform/loadWaveForm_terminate.h"
#include "tmwtypes.h"
#include <unistd.h>
#include <semaphore.h>

#include "syncFrame/SyncFrame.h"
#include "syncFrame/SyncFrameInit.h"

#include "timing/tic.h"
#include "timing/toc.h"

#include "trx/rx.h"

void sync_thread();
//void rx_data_thread();
void pdcch_thread();

creal32_T rxWaveform[FRAME_BUFF_SIZE];
creal32_T data[1916];

int idx;
int offset = 0;
int done = 1;
int syncronized = 0;
sem_t get_frame_offset;
//sem_t synched;


int main(void){
	idx = 0;

	pthread_t sync_id;
	pthread_t rx_data_id;
	pthread_t pdcch_id;
	pthread_t rx_id;

	create_args();
	init_usrp();

	sem_init(&get_frame_offset, 0,0);
	//sem_init(&synched, 0,0);

	pthread_create(&sync_id, NULL, sync_thread, (void *)&sync_id);
	//pthread_create(&rx_data_id, NULL, rx_data_thread, (void *)&rx_data_id);
	pthread_create(&rx_id, NULL, stream_rx_thread, (void *)&rx_id);
	pthread_create(&pdcch_id, NULL, pdcch_thread, (void *)&pdcch_id);


	pthread_join(rx_id,NULL);
	pthread_join(sync_id,NULL);
	pthread_join(pdcch_id,NULL);
	sem_destroy(&get_frame_offset);
	//sem_destroy(&synched);
	terminate();
	//loadWaveForm_terminate();
	return 0;
}


void sync_thread(){
	//Might have shared data issues if dont make a copy of rxWaveform
	creal32_T syncWave[6584];
	creal32_T initWave[FRAME_BUFF_SIZE];
	int x;
	int new_offset;
	//Wait until a whole frame has been recieved then find the initial offset
	sem_wait(&get_frame_offset);
	printf("Index: %d\n", idx);

	for(x = 0; x <FRAME_BUFF_SIZE; x++){
		initWave[x] = rxWaveform[x];
	}


	offset= SyncFrameInit(initWave);
	syncronized = 1;
	printf("Initial Frame sync at %i\n", offset);


	//After recieving the first 6584 samples of a frame (symbols 0-3) resync with frame
	while(done != 0){
		sem_wait(&get_frame_offset);
		printf("Index: %d\n", idx);
		for(x=0;x<6584;x++){
			syncWave[x] = rxWaveform[(offset+x+FRAME_BUFF_SIZE)%FRAME_BUFF_SIZE];
		}
		new_offset = SyncFrame(syncWave);
		printf("Frame sync at %i\n", new_offset);
		offset += new_offset;
	}
}


//void rx_data_thread(){
//	printf("Begining rx data thread\n");
//
//	int x;
//	int get_sync = 3;
//	for(x = 0; x < 23040; x++){
//		idx = (idx+2000)%2580298;
//		if(idx > 6584 && get_sync == 1){
//			get_sync =0;
//			printf("Get Offset\n");
//			sem_post(&get_frame_offset);
//		}
//		else if(idx <6584 && get_sync ==0){
//			get_sync =1;
//		}else if(idx > FRAME_BUFF_SIZE && get_sync == 3){
//			get_sync = 0;
//			printf("Get Offset\n");
//			sem_post(&get_frame_offset);
//		}
//		usleep(43);
//	}
//	printf("Done recieving Data\n");
//	done = 0;
//}




void pdcch_thread(){
	printf("Begining pdcch thread\n");

}