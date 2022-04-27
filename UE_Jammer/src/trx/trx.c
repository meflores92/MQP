/*
 =====================================================================================
 Name        : trx.c
 Author      : Devon Poisson, Colin Stevens, and Maya Flores 
 Copyright   : Please note that the examples found on Ettus' GitHub 
               (https://github.com/EttusResearch/uhd/tree/master/host/examples) were 
			   modified to create this source file. Code was taken from 
			   rx_samples_c.c and tx_samples_c.c prior to modification.    
 Description : Source file containing USRP initialization function, the receive thread, 
               and the transmit thread.
 ======================================================================================
 */

/*================================= Include Files =====================================*/
//Include standard libraries 
#include "getopt.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <complex.h>
#include <unistd.h>
#include <math.h>
//Include UHD Library 
#include <uhd.h>
//Include MATLAB timing 
#include "../timing/tic.h"
#include "../timing/toc.h"
//Include other header files
#include "trx.h"
#include "../common.h"
/*=====================================================================================*/


/*================================= Global Variables ==================================*/
//USRP Parameters
double freq = center_freq;
double rate = 46.08e6;
double rx_gain = 50.00;
double tx_gain = 89.750;
double bandwidth = 40.00e6; // Default for B210 == 56.00Mhz
double bandwidth_out;
char *device_args = "num_recv_frames = 256, recv_frame_size = 7680, num_send_frames = 256, send_frame_size = 7680"; // originally 7680
size_t channel = 0;
char *clk_string = "external";
char *time_string = "external";

//USRP
uhd_usrp_handle usrp;
uhd_rx_streamer_handle rx_streamer;
uhd_tx_streamer_handle tx_streamer;
uhd_rx_metadata_handle rx_md;
uhd_tx_metadata_handle tx_md;

//RX and TX buffers
size_t rx_samps_per_buff;
float *rx_buff = NULL;
void **rx_buffs_ptr = NULL;
size_t tx_samps_per_buff;
float *tx_buff = NULL;
const void **tx_buffs_ptr = NULL;

//Slot sample look up table
const int slot_lut[40] = {
		0, 23040, 46080, 69120, 92160, 115200, 138240,
		161280, 184320, 207361, 230400, 253440, 276480,
		299520, 322560, 345600, 368640, 391680, 414721,
		437760, 460800, 483841, 506880, 529920, 552960,
		576000, 599040, 622080, 645120, 668160, 691200,
		714240, 737280, 760320, 783360, 806401, 829441,
		852480, 875520, 898560
	};


// Structures required by USRP
uhd_tune_request_t tune_request = {
	.target_freq = center_freq,
	.rf_freq_policy = UHD_TUNE_REQUEST_POLICY_AUTO,
	.dsp_freq_policy = UHD_TUNE_REQUEST_POLICY_AUTO,
};
uhd_tune_result_t tune_result;

uhd_stream_args_t stream_args = {
	.cpu_format = "fc32",
	.otw_format = "sc16",
	.args = "spp= 1916", // 1916
	.channel_list = &channel,
	.n_channels = 1};

uhd_stream_cmd_t stream_cmd = {
	.stream_mode = UHD_STREAM_MODE_START_CONTINUOUS,
	//.num_samps = n_samples,
	.stream_now = true};

uhd_stream_cmd_t stream_cmd_term = {
	.stream_mode = UHD_STREAM_MODE_STOP_CONTINUOUS,
	//.num_samps = n_samples,
	.stream_now = true
	//.time_spec_full_secs = 1
};

//Flags
int doneTransmittingFlag =1;
int load_signal = 1;
int get_sync = 0;
int updatePDCCHPercentFlag=0;

//Error handling 
int return_code = EXIT_SUCCESS;
char error_string[512];

//Performance
float PDCCHPercent = 0.0;
/*=====================================================================================*/


/*=========================== USRP Initilization Functions ============================*/
// Set-up USRP Parameters
void create_args(){
	// Create USRP Handle
	fprintf(stderr, "Creating USRP with args \"%s\"...\n", device_args);
	uhd_usrp_make(&usrp, device_args);

	// Create Streamers
	uhd_rx_streamer_make(&rx_streamer);
	uhd_tx_streamer_make(&tx_streamer);

	// Create metadata
	uhd_rx_metadata_make(&rx_md);
	uhd_tx_metadata_make(&tx_md, false, 0, 0.1, true, false);
}

// Initialize USRP Receiver Clock Source, Sample Rate, Gain, etc.
void init_usrp_rx(){
	char mboard_name[512];
	size_t num_mboards_out;
	char time_source_out[512];
	char clock_source_out[512];

	// Check Number of Motherboards
	fprintf(stderr, "Checking Number of Motherboards\n");
	uhd_usrp_get_num_mboards(usrp, &num_mboards_out);
	fprintf(stderr, "	Number of Motherboards: %d\n", (int)num_mboards_out);

	// Check Motherboard Name
	fprintf(stderr, "Checking Motherboard Name\n");
	uhd_usrp_get_mboard_name(usrp, mboard, mboard_name, 512);
	fprintf(stderr, "	Motherboard name: %s\n", mboard_name);

	// Set Clock Source
	fprintf(stderr, "Setting Clock Source\n");
	uhd_usrp_set_clock_source(usrp, clk_string, mboard);

	// Check Clock Source
	fprintf(stderr, "Checking Clock Source\n");
	uhd_usrp_get_clock_source(usrp, 0, clock_source_out, 512);
	fprintf(stderr, "	Clock Source: %s\n", clock_source_out);

	// Set Time Source
	fprintf(stderr, "Setting Time Source\n");
	uhd_usrp_set_time_source(usrp, time_string, mboard);

	// Check Time source
	fprintf(stderr, "Checking Time Source\n");
	uhd_usrp_get_time_source(usrp, 0, time_source_out, 512);
	fprintf(stderr, "	Time Source: %s\n", time_source_out);

	// Set rate
	fprintf(stderr, "Setting RX Rate: %f...\n", rate);
	uhd_usrp_set_rx_rate(usrp, rate, channel);

	// Check Rate
	uhd_usrp_get_rx_rate(usrp, channel, &rate);
	fprintf(stderr, "	Actual RX Rate: %f...\n", rate);

	// Set Gain
	fprintf(stderr, "Setting RX Gain: %f dB...\n", rx_gain);
	uhd_usrp_set_rx_gain(usrp, rx_gain, channel, "");

	// Check Gain
	uhd_usrp_get_rx_gain(usrp, channel, "", &rx_gain);
	fprintf(stderr, "	Actual RX Gain: %f...\n", rx_gain);

	// Set Freq
	fprintf(stderr, "Setting RX frequency: %f MHz...\n", center_freq / 1e6);
	uhd_usrp_set_rx_freq(usrp, &tune_request, channel, &tune_result);

	// Check Freq
	uhd_usrp_get_rx_freq(usrp, channel, &freq);
	fprintf(stderr, "	Actual RX frequency: %f MHz...\n", freq / 1e6);

	// Set RX Bandwidth
	fprintf(stderr, "Setting RX Bandwidth: %f MHz\n", bandwidth / 1e6);
	uhd_usrp_set_rx_bandwidth(usrp, bandwidth, channel);

	// Check RX Bandwidth
	fprintf(stderr, "Checking RX Bandwidth...\n");
	uhd_usrp_get_rx_bandwidth(usrp, channel, &bandwidth_out);
	fprintf(stderr, "	Actual RX Bandwidth: %f MHz\n", bandwidth_out / 1e6);

	// Set up streamer
	stream_args.channel_list = &channel;
	uhd_usrp_get_rx_stream(usrp, &stream_args, rx_streamer);

	// Set-up rx buffer
	uhd_rx_streamer_max_num_samps(rx_streamer, &rx_samps_per_buff);
	fprintf(stderr, "Buffer size in samples: %zu\n", rx_samps_per_buff);
	rx_buff = malloc(rx_samps_per_buff * 2 * sizeof(float));
	rx_buffs_ptr = (void **)&rx_buff;
}

// Initialize USRP Transmitter Clock Source, Sample Rate, Gain, etc.
void init_usrp_tx(){
	// Set rate
	fprintf(stderr, "Setting TX Rate: %f...\n", rate);
	uhd_usrp_set_tx_rate(usrp, rate, channel);

	// See what rate actually is
	uhd_usrp_get_tx_rate(usrp, channel, &rate);
	fprintf(stderr, "Actual TX Rate: %f...\n\n", rate);

	// Set gain
	fprintf(stderr, "Setting TX Gain: %f db...\n", tx_gain);
	uhd_usrp_set_tx_gain(usrp, tx_gain, 0, "");

	// Check gain
	uhd_usrp_get_tx_gain(usrp, channel, "", &tx_gain);
	fprintf(stderr, "Actual TX Gain: %f...\n", tx_gain);

	// Set frequency
	fprintf(stderr, "Setting TX frequency: %f MHz...\n", freq / 1e6);
	uhd_usrp_set_tx_freq(usrp, &tune_request, channel, &tune_result);

	// Check frequency
	uhd_usrp_get_tx_freq(usrp, channel, &freq);
	fprintf(stderr, "Actual TX frequency: %f MHz...\n", freq / 1e6);

	//Set Bandwitdh 
	fprintf(stderr, "Setting TX Bandwidth: %f MHz\n", bandwidth / 1e6);
	uhd_usrp_set_tx_bandwidth(usrp, bandwidth, channel);

	// Check Bandwidth
	fprintf(stderr, "Checking TX Bandwidth...\n");
	uhd_usrp_get_tx_bandwidth(usrp, channel, &bandwidth_out);
	fprintf(stderr, "	Actual TX Bandwidth: %f MHz\n", bandwidth_out / 1e6);

	//Set up streamer
	stream_args.channel_list = &channel;

	uhd_usrp_get_tx_stream(usrp, &stream_args, tx_streamer);

	//Set up tx buffer
	uhd_tx_streamer_max_num_samps(tx_streamer, &tx_samps_per_buff);
	fprintf(stderr, "Buffer size in samples: %zu\n", tx_samps_per_buff);
	tx_buff = calloc(sizeof(float), tx_samps_per_buff * 2);
	tx_buffs_ptr = (const void **)&tx_buff;
}
/*=====================================================================================*/


/*==================================== RX Thread ======================================*/
// Receive thread used to capture data from USRP and signal other threads
void *stream_rx_thread(){
	int x = 0;
	int counter = 0;
	int decode_attemps = 0;
	int count = 0;
	int missed_slot = 0;
	int count_sync = 0;
	int getPDCCHPerformance=0;
	int overflows=0;
	int stable=0;
	uhd_rx_metadata_error_code_t error_code;

	// Issue stream command
	fprintf(stderr, "Issuing stream command.\n");
	uhd_rx_streamer_issue_stream_cmd(rx_streamer, &stream_cmd);


	// Streaming Loop
	while (done == 1){
		size_t num_rx_samps = 0;

		//  Recv Packet
		uhd_rx_streamer_recv(rx_streamer, rx_buffs_ptr, rx_samps_per_buff, &rx_md, 0.1, false, &num_rx_samps); // Default: one_packet = false

		// Check for Error Code
		uhd_rx_metadata_error_code(rx_md, &error_code);

		//Handle error Condition
		if(error_code != UHD_RX_METADATA_ERROR_CODE_NONE){
			if(error_code == UHD_RX_METADATA_ERROR_CODE_OVERFLOW){
				// printf("Overflow!\n");
				overflows++;
			}
			else{
				fprintf(stderr, "Error code 0x%x was returned during streaming. Aborting.\n", error_code);
				int64_t full_secs;
				double frac_secs;
				uhd_rx_metadata_time_spec(rx_md, &full_secs, &frac_secs);
				fprintf(stderr, "Received packet: %zu samples, %.f full secs, %f frac secs\n",
						num_rx_samps,
						difftime(full_secs, (int64_t) 0),
						frac_secs);
				return_code = EXIT_FAILURE;

				done = TRUE;
			}
		}else if(overflows != 0){
			overflows = 0;
			stable = 0;
			get_sync = 3;
		}

		// Copy received data from the USRP host buffer to a global buffer for processing
		int y = 0;
		for (x = 0; x < 3832; x += 2){
			rxWaveform[(y + idx) % FRAME_BUFF_SIZE].im = rx_buff[x + 1];
			rxWaveform[(y + idx) % FRAME_BUFF_SIZE].re = rx_buff[x];
			y++;
		}

		idx = (idx + 1916) % FRAME_BUFF_SIZE;
		
		if (syncronized == 1){
			//State machine to control PDCCH thread signaling 
			if(count_sync == 0 && idx > offset && idx <((offset+2000)%FRAME_BUFF_SIZE)){
				count = 0;
				count_sync =1;
			}
			if(idx > ((offset+slot_lut[count]+delta+FRAME_BUFF_SIZE)%FRAME_BUFF_SIZE) && idx < ((offset+slot_lut[(count)%40]+delta+FRAME_BUFF_SIZE)%FRAME_BUFF_SIZE+6000)&& count_sync == 1){
				decode_attemps++;
				if(done_decoding == 1){
					slot = count;
					sem_post(&decode_pdcch);
				}
				else if(done_decoding == 0){
					missed_slot ++;
				}
				if(count == 0){
					getPDCCHPerformance++;
					if(getPDCCHPerformance > 50){
						if(decode_attemps == 0){
							PDCCHPercent = 0;
						}else{
							PDCCHPercent = (1-((float)missed_slot/decode_attemps))*100;
						}
						missed_slot = 0;
						decode_attemps = 0;
						getPDCCHPerformance= 0;
						updatePDCCHPercentFlag = 1;
					}
				}
				count = (count + 1)%40;
			}

			// Signal tx thread when there are PUSCH transmissions scheduled
			if (head != next && doneTransmittingFlag == 1){
				if (idx > (offset + slot_lut[(pusch_fifo[head].slot + k2 + PUSCHOffset) % 40] - latency + FRAME_BUFF_SIZE) % FRAME_BUFF_SIZE && idx < (offset + slot_lut[(pusch_fifo[head].slot + k2+PUSCHOffset) % 40]) % FRAME_BUFF_SIZE){
					tx_idx = idx;
					doneTransmittingFlag =0;
					sem_post(&transmit);
				}
			}
		}

		//"State Machine" to control frame syncronization signaling 
		if(syncronized == 3 && get_sync==4){
			syncronized = 1;
			get_sync = 1;
		}
		if (idx > 919680 && get_sync == 0){ // 919680  458874
			get_sync = 1;
			sem_post(&get_frame_offset);
		}
		else if (idx < ((6584 + offset + FRAME_BUFF_SIZE) % FRAME_BUFF_SIZE) && get_sync == 1){
			if (syncronized == 1){
				get_sync = 2;
			}
		}
		else if (idx > ((6584 + offset + FRAME_BUFF_SIZE) % FRAME_BUFF_SIZE) && get_sync == 2){
			get_sync = 1;
			counter++;
			sem_post(&get_frame_offset);
		}
		else if(get_sync == 3 && syncronized != 0){
			stable++;
			if(stable> 500){
				syncronized = 0;
				sem_post(&get_frame_offset);
				stable = 0;
				get_sync=4;
			}
		}
	}

	//Post all semaphores for clean program termination
	sem_post(&get_frame_offset);
	sem_post(&decode_pdcch);
	sem_post(&transmit);
	sem_post(&decode_dci);
	sem_post(&plotData);
	sem_post(&plotSpec);
	sem_post(&filter);
	sem_post(&detected);
	return NULL;
}
/*=====================================================================================*/


/*================================== TX Thread ========================================*/
//Thread used to interface with USRP and send jamming signal
void *stream_tx_thread(){
	uint64_t num_acc_samps = 0;
	size_t num_samps_sent = 0;
	const int slot_lut[40] = {
			0, 23040, 46080, 69120, 92160, 115200, 138240,
			161280, 184320, 207361, 230400, 253440, 276480,
			299520, 322560, 345600, 368640, 391680, 414721,
			437760, 460800, 483841, 506880, 529920, 552960,
			576000, 599040, 622080, 645120, 668160, 691200,
			714240, 737280, 760320, 783360, 806401, 829441,
			852480, 875520, 898560
		};
	int i = 0;
	int index = 0;
	int tx_index = 0;

	//Continue thread execution until program terminates
	while (done != 0){
		sem_wait(&transmit);
		if(done != 0){
			num_acc_samps = 0;
			num_samps_sent = 0;
			tx_index = 0;

			//Set start of burst to true and end of burst to false (set-up burst mode)
			uhd_tx_metadata_make(&tx_md, false, 0, 0.1, true, false);

			index = 0;

			//Transmitt data until the number of sample sent is equal to the size of the PUSCH signal (defined by NUM_TX_SAMPS)  
			while (num_acc_samps <= NUM_TX_SAMPS){

				//Loop to load jamming signal into tx buffer
				for (i = 0; i < (tx_samps_per_buff * 2); i += 2){
					if((num_acc_samps+(i/2)) < 1646*13 && (tx_idx + (i/2)) > ((offset + slot_lut[(pusch_fifo[head].slot + k2 + PUSCHOffset) % 40] - latency + FRAME_BUFF_SIZE) % FRAME_BUFF_SIZE)){
						tx_buff[i] = pusch_fifo[head].waveform[index%(1536*2)];
						index++;
						tx_buff[i+1] = pusch_fifo[head].waveform[index%(1536*2)];
						index++;
					}else{
						tx_buff[i] = 0.0;
						tx_buff[i+1] = 0.0;
						index+=2;
					}
				}

				tx_index = (tx_index + (1916 * 2));
				tx_idx += 1916;

				//Burst Mode Control
				if (num_acc_samps == (NUM_TX_SAMPS - 1916)){
					uhd_tx_metadata_make(&tx_md, false, 0, 0.1, false, true);
				}
				else if (num_acc_samps == 0){
					uhd_tx_metadata_make(&tx_md, false, 0, 0.1, false, false);
				}

				//Send data in tx buffer
				uhd_tx_streamer_send(tx_streamer, tx_buffs_ptr, tx_samps_per_buff, &tx_md, 0.1, &num_samps_sent);

				//Latency adjustment
				if((tx_idx - 1916) > ((offset + slot_lut[(pusch_fifo[head].slot + k2 + PUSCHOffset) % 40] - latency + FRAME_BUFF_SIZE) % FRAME_BUFF_SIZE)){
					num_acc_samps += num_samps_sent;
				}

			}

			head = (head + 1) % 40;
			doneTransmittingFlag = 1;

			if(plot_spec_flag == 1){
				sem_post(&plotSpec);
			}
		}
	}
	return NULL;
}
/*=====================================================================================*/


/*============================== USRP Terminate Fuction ===============================*/
//Stops USRP streaming and frees memory 
int terminate(){
	int64_t full_secs;
	double frac_secs;
 
	//Stop all USRP streaming
	fprintf(stderr, "\nIssuing stop stream command.\n");
	uhd_rx_streamer_issue_stream_cmd(rx_streamer, &stream_cmd_term);

	// Display Run Time
	uhd_rx_metadata_time_spec(rx_md, &full_secs, &frac_secs);
	fprintf(stderr, "Execution Time: %.f full secs, %f frac secs\n",
			difftime(full_secs, (int64_t)0),
			frac_secs);

	//Free all dynamically allocated memory associated with USRP
	fprintf(stderr, "Freeing buffers.\n");
	free(rx_buff);
	free(tx_buff);
	rx_buff = NULL;
	rx_buffs_ptr = NULL;
	tx_buff = NULL;
	tx_buffs_ptr = NULL;

	fprintf(stderr, "Cleaning up streamers.\n");
	uhd_rx_streamer_free(&rx_streamer);
	uhd_tx_streamer_free(&tx_streamer);

	fprintf(stderr, "Cleaning up RX metadata.\n");
	uhd_rx_metadata_free(&rx_md);
	uhd_tx_metadata_free(&tx_md);

	fprintf(stderr, "Cleaning up USRP.\n");
	if (return_code != EXIT_SUCCESS && usrp != NULL)
	{
		uhd_usrp_last_error(usrp, error_string, 512);
		fprintf(stderr, "USRP reported the following error: %s\n", error_string);
	}
	uhd_usrp_free(&usrp);

	fprintf(stderr, (return_code ? "Failure\n" : "Success\n"));
	return return_code;
}
/*=====================================================================================*/

/*=========================== USRP Values from Command Line  ==========================*/
void updateArgs(int argc, char* argv[]){
	for (int i=1; i<argc; i+=2){
		 if(strcmp(argv[i],"--freq") == 0){
			 freq = atof(argv[i+1]);
		 }
		 else if(strcmp(argv[i],"--tx_gain") == 0){
			 tx_gain = atof(argv[i+1]);
		 }
		 else if(strcmp(argv[i],"--rx_gain") == 0){
			 rx_gain = atof(argv[i+1]);
		 }
		 else if(strcmp(argv[i],"--rate") == 0){
			 rate = atof(argv[i+1]);
		 }
		 else if(strcmp(argv[i],"--clk_source") == 0){
			 if((int) atof(argv[i+1]) == 0){
				clk_string = "internal";
			 }
			 else if((int) atof(argv[i+1]) == 1){
				clk_string = "external";
			 }
		 }
		 else if(strcmp(argv[i],"--time_source") == 0){
			 if((int) atof(argv[i+1]) == 0){
				time_string = "internal";
			 }
			 else if((int) atof(argv[i+1]) == 1){
				time_string = "external";
			 }
		 }
		 else{
			 return_code = EXIT_FAILURE;
		 }

	}
}
/*=====================================================================================*/