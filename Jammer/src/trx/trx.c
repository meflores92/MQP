/*
 ============================================================================
 Name        : trx
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : Receive signal with USRP and save to file for post processing in MATLAB
 ============================================================================
 */

// Includes
#include <uhd.h>
#include "getopt.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <complex.h>
#include <unistd.h>
#include <math.h>

#include "../timing/tic.h"
#include "../timing/toc.h"

#include "trx.h"

#define latency 12668//10000//8407 //12053

// Global Variables
double freq = center_freq;
double rate = 46.08e6;
double rx_gain = 60.0;
double tx_gain = 90.0;
double bandwidth = 40.00e6; // Default for B210 == 56.00Mhz
double bandwidth_out;
float PDCCHPercent = 0.0;
int updatePDCCHPercentFlag=0;

int doneTransmittingFlag =1;
int load_signal = 1;
int get_sync = 0;

char *device_args = "serial=31EEB51, num_recv_frames = 256, recv_frame_size = 7680, num_send_frames = 256, send_frame_size = 7680"; // originally 7680
size_t channel = 0;

int return_code = EXIT_SUCCESS;
char error_string[512];
uint64_t total_num_samps = 0;

uhd_usrp_handle usrp;
uhd_rx_streamer_handle rx_streamer;
uhd_tx_streamer_handle tx_streamer;
uhd_rx_metadata_handle rx_md;
uhd_tx_metadata_handle tx_md;

size_t rx_samps_per_buff;
float *rx_buff = NULL;
void **rx_buffs_ptr = NULL;

size_t tx_samps_per_buff;
float *tx_buff = NULL;
const void **tx_buffs_ptr = NULL;


const int slot_lut[40] = {
		0, 23040, 46080, 69120, 92160, 115200, 138240,
		161280, 184320, 207361, 230400, 253440, 276480,
		299520, 322560, 345600, 368640, 391680, 414721,
		437760, 460800, 483841, 506880, 529920, 552960,
		576000, 599040, 622080, 645120, 668160, 691200,
		714240, 737280, 760320, 783360, 806401, 829441,
		852480, 875520, 898560
	};


// Create other necessary structs
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

// Set-up USRP Parameters
int create_args()
{
	// Create USRP Handle
	fprintf(stderr, "Creating USRP with args \"%s\"...\n", device_args);
	uhd_usrp_make(&usrp, device_args);

	// Create Streamers
	uhd_rx_streamer_make(&rx_streamer);
	uhd_tx_streamer_make(&tx_streamer);

	// Create metadata
	uhd_rx_metadata_make(&rx_md);
	uhd_tx_metadata_make(&tx_md, false, 0, 0.1, true, false);

	return EXIT_SUCCESS;
}

// Initialize USRP Clock Source, Sample Rate, Gain, etc.
int init_usrp_rx()
{
	char mboard_name[512];
	size_t num_mboards_out;
	char *clk_string = "external";
	char time_source_out[512];
	char *time_string = "external";
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

	// Set up buffer
	uhd_rx_streamer_max_num_samps(rx_streamer, &rx_samps_per_buff);
	fprintf(stderr, "Buffer size in samples: %zu\n", rx_samps_per_buff);
	rx_buff = malloc(rx_samps_per_buff * 2 * sizeof(float));
	rx_buffs_ptr = (void **)&rx_buff;

	return EXIT_SUCCESS;
}

void init_usrp_tx()/*
 ============================================================================
 Name        : trx
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : Receive signal with USRP and save to file for post processing in MATLAB
 ============================================================================
 */

// Includes
#include <uhd.h>
#include "getopt.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <complex.h>
#include <unistd.h>
#include <math.h>

#include "../timing/tic.h"
#include "../timing/toc.h"

#include "trx.h"

{
	// Set rate
	fprintf(stderr, "Setting TX Rate: %f...\n", rate);
	uhd_usrp_set_tx_rate(usrp, rate, channel);

	// See what rate actually is
	uhd_usrp_get_tx_rate(usrp, channel, &rate);
	fprintf(stderr, "Actual TX Rate: %f...\n\n", rate);

	// Set gain
	fprintf(stderr, "Setting TX Gain: %f db...\n", tx_gain);
	uhd_usrp_set_tx_gain(usrp, tx_gain, 0, "");

	// See what gain actually is
	uhd_usrp_get_tx_gain(usrp, channel, "", &tx_gain);
	fprintf(stderr, "Actual TX Gain: %f...\n", tx_gain);

	// Set frequency
	fprintf(stderr, "Setting TX frequency: %f MHz...\n", freq / 1e6);
	uhd_usrp_set_tx_freq(usrp, &tune_request, channel, &tune_result);

	// See what frequency actually is
	uhd_usrp_get_tx_freq(usrp, channel, &freq);
	fprintf(stderr, "Actual TX frequency: %f MHz...\n", freq / 1e6);

	fprintf(stderr, "Setting TX Bandwidth: %f MHz\n", bandwidth / 1e6);
	uhd_usrp_set_tx_bandwidth(usrp, bandwidth, channel);

	// Check RX Bandwidth
	fprintf(stderr, "Checking TX Bandwidth...\n");
	uhd_usrp_get_tx_bandwidth(usrp, channel, &bandwidth_out);
	fprintf(stderr, "	Actual TX Bandwidth: %f MHz\n", bandwidth_out / 1e6);

	// Set up streamer
	stream_args.channel_list = &channel;

	uhd_usrp_get_tx_stream(usrp, &stream_args, tx_streamer);

	// Set up buffer
	uhd_tx_streamer_max_num_samps(tx_streamer, &tx_samps_per_buff);

	fprintf(stderr, "Buffer size in samples: %zu\n", tx_samps_per_buff);
	tx_buff = calloc(sizeof(float), tx_samps_per_buff * 2);
	tx_buffs_ptr = (const void **)&tx_buff;
	size_t i = 0;
	for (i = 0; i < (tx_samps_per_buff * 2); i += 2)
	{
		tx_buff[i] = 0.1f;
		tx_buff[i + 1] = 0;
	}
}

// Streaming Function
void *stream_rx_thread()
{
	int getPDCCHPerformance=0;
	int overflows=0;
	int stable=0;

	int x = 0;
	int counter = 0;
	int decode_attemps = 0;
	int count = 0;
	int missed_slot = 0;
	int count_sync = 0;
	

	// Issue stream command
	fprintf(stderr, "Issuing stream command.\n");
	uhd_rx_streamer_issue_stream_cmd(rx_streamer, &stream_cmd);

	uhd_rx_metadata_error_code_t error_code;
	size_t total_purge_samps = 0;

	while (total_purge_samps <= 20000000)
	{
		size_t num_rx_samps = 0;
		uhd_rx_streamer_recv(rx_streamer, rx_buffs_ptr, rx_samps_per_buff, &rx_md, 0.1, false, &num_rx_samps);

		total_purge_samps += num_rx_samps;
	}

	// Streaming Loop
	while (done != 0){
		size_t num_rx_samps = 0;

		//  Recv Packet
		uhd_rx_streamer_recv(rx_streamer, rx_buffs_ptr, rx_samps_per_buff, &rx_md, 0.1, false, &num_rx_samps); // Default: one_packet = false

		// Check for Error Code
		uhd_rx_metadata_error_code(rx_md, &error_code);

		//Handle error Condition
		if(error_code != UHD_RX_METADATA_ERROR_CODE_NONE){
			if(error_code == UHD_RX_METADATA_ERROR_CODE_OVERFLOW){
				printf("Overflow!\n");
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

				done = 0;
			}
		}else if(overflows != 0){
			overflows = 0;
			stable = 0;
			get_sync = 3;
		}


		// Handle Data
		int y = 0;
		for (x = 0; x < 3832; x += 2){
			rxWaveform[(y + idx) % FRAME_BUFF_SIZE].im = rx_buff[x + 1];
			rxWaveform[(y + idx) % FRAME_BUFF_SIZE].re = rx_buff[x];
			y++;
		}

		idx = (idx + 1916) % FRAME_BUFF_SIZE;

		if (syncronized == 1){
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

			// Signal Transmit Thread when there are PUSCH Transmission Scheduled
			if (head != next && doneTransmittingFlag == 1){
				if (idx > (offset + slot_lut[(pusch_fifo[head].slot + k2 + PUSCHOffset) % 40] - latency + FRAME_BUFF_SIZE) % FRAME_BUFF_SIZE && idx < (offset + slot_lut[(pusch_fifo[head].slot + k2+PUSCHOffset) % 40]) % FRAME_BUFF_SIZE){
					tx_idx = idx;
					//target_slot = slot_lut[(pusch_fifo[head].slot + k2) % 40];
					// printf("Target Slot: %d\n", target_slot);
					//head = (head + 1) % 40;
					//printf("Target Slot: %i\n", (pusch_fifo[head].slot + k2 + PUSCHOffset) % 40);
					//printf("Samples from Index to Target Slot: %i\n", idx-(offset + slot_lut[(pusch_fifo[head].slot + k2 + PUSCHOffset) % 40]) % FRAME_BUFF_SIZE);
					doneTransmittingFlag =0;
					sem_post(&transmit);
				}
			}
		}

		if(syncronized == 3 && get_sync==4){
			syncronized = 1;
			get_sync = 1;
		}
		if (idx > 919680 && get_sync == 0){ // 919680  458874
			get_sync = 1;
			// printf("Init Offset\n");
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
			// printf("Get Offset\n");
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


void *stream_tx_thread()
{
	uint64_t num_acc_samps = 0;
	size_t num_samps_sent = 0;
	int i = 0;

	int index = 0;
	// int y = 0;
	// int x = 0;
	useconds_t t = 0;
	useconds_t tx_latency = 1;
	const int slot_lut[40] = {
			0, 23040, 46080, 69120, 92160, 115200, 138240,
			161280, 184320, 207361, 230400, 253440, 276480,
			299520, 322560, 345600, 368640, 391680, 414721,
			437760, 460800, 483841, 506880, 529920, 552960,
			576000, 599040, 622080, 645120, 668160, 691200,
			714240, 737280, 760320, 783360, 806401, 829441,
			852480, 875520, 898560
		};
	int tx_index = 0;
	int debug_flag = 1;

	// Send Data
	// fprintf(stderr, "Send Data\n");
	while (done != 0)
	{
		sem_wait(&transmit);
		if(done != 0){
			num_acc_samps = 0;
			num_samps_sent = 0;
			tx_index = 0;
			// int loop_count = 0;

			uhd_tx_metadata_make(&tx_md, false, 0, 0.1, true, false);

			//1646
			if (debug_flag == 0){
				t = (useconds_t)ceil(1e6 * (target_slot - tx_index) / rate);

				if (t > tx_latency)
				{
					t = t - tx_latency;
				}
				usleep(t);
			}
			index = 0;
			while (num_acc_samps <= NUM_TX_SAMPS){


				for (i = 0; i < (tx_samps_per_buff * 2); i += 2){
					//tx_buff[i] = jamming_signals[Lrb_idx[pusch_fifo[head].Lrb-1]][i + tx_index + 1];
					//tx_buff[i + 1] = jamming_signals[Lrb_idx[pusch_fifo[head].Lrb - 1 + pusch_fifo[head].RBstart]][i + tx_index];

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

				// loop_count++;

				if (num_acc_samps == (NUM_TX_SAMPS - 1916)){
					uhd_tx_metadata_make(&tx_md, false, 0, 0.1, false, true);
				}

				uhd_tx_streamer_send(tx_streamer, tx_buffs_ptr, tx_samps_per_buff, &tx_md, 0.1, &num_samps_sent);

				if (num_acc_samps == 0){
					uhd_tx_metadata_make(&tx_md, false, 0, 0.1, false, false);
				}

				if((tx_idx - 1916) > ((offset + slot_lut[(pusch_fifo[head].slot + k2 + PUSCHOffset) % 40] - latency + FRAME_BUFF_SIZE) % FRAME_BUFF_SIZE)){
					num_acc_samps += num_samps_sent;
				}

			}

			head = (head + 1) % 40;
			doneTransmittingFlag = 1;
			if(plot_spec_flag == 1){
				sem_post(&plotSpec);
			}
			// printf("Transmiter Loop Count: %d\n", loop_count);
			// fprintf(stderr, "Sent %zu samples\n", num_samps_sent);
		}
	}
	return NULL;
}

int terminate()
{
	int64_t full_secs;
	double frac_secs;

	fprintf(stderr, "Issuing stop stream command.\n");
	uhd_rx_streamer_issue_stream_cmd(rx_streamer, &stream_cmd_term);

	// Display Run Time
	uhd_rx_metadata_time_spec(rx_md, &full_secs, &frac_secs);
	fprintf(stderr, "Execution Time: %.f full secs, %f frac secs\n",
			difftime(full_secs, (int64_t)0),
			frac_secs);

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
