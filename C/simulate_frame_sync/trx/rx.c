/*
 ============================================================================
 Name        : rx_testing
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : Receive signal with USRP and save to file for post processing in MATLAB
 ============================================================================
 */

//Includes
#include <uhd.h>
#include "getopt.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <complex.h>
#include "rx.h"


//Global Variables


double freq= center_freq;
double rate = 46.08e6;
double gain = 70.0;
double bandwidth = 40.00e6; //Default for B210 == 56.00Mhz
double bandwidth_out;

char* device_args = "serial=3164073, num_recv_frames = 256, recv_frame_size = 7680"; //originally null
size_t channel = 0;


int return_code = EXIT_SUCCESS;
char error_string[512];


uhd_rx_streamer_handle rx_streamer;
uhd_usrp_handle usrp;
uhd_rx_metadata_handle md;

size_t samps_per_buff;
float *buff = NULL;
void **buffs_ptr = NULL;

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
		.args = "spp= 1916", //1916
		.channel_list = &channel,
		.n_channels = 1
};

uhd_stream_cmd_t stream_cmd = {
		.stream_mode = UHD_STREAM_MODE_START_CONTINUOUS,
		//.num_samps = n_samples,
		.stream_now = true
};

uhd_stream_cmd_t stream_cmd_term = {
		.stream_mode = UHD_STREAM_MODE_STOP_CONTINUOUS,
		//.num_samps = n_samples,
		.stream_now = true
		//.time_spec_full_secs = 1
};


//Set-up USRP Parameters
int create_args() {
	// Create USRP Handle
	fprintf(stderr, "Creating USRP with args \"%s\"...\n", device_args);
	uhd_usrp_make(&usrp, device_args);

	// Create RX Streamer
	uhd_rx_streamer_make(&rx_streamer);

	// Create RX metadata
	uhd_rx_metadata_make(&md);

	return EXIT_SUCCESS;
}

//Initialize USRP Clock Source, Sample Rate, Gain, etc.
int init_usrp(){
	char mboard_name[512];
	size_t num_mboards_out;
	char *clk_string = "external";
	char time_source_out[512];
	char *time_string = "external";
	char clock_source_out[512];

	//Check Number of Motherboards
	fprintf(stderr, "Checking Number of Motherboards\n");
	uhd_usrp_get_num_mboards(usrp, &num_mboards_out);
	fprintf(stderr, "	Number of Motherboards: %d\n", (int)num_mboards_out);

	//Check Motherboard Name
	fprintf(stderr, "Checking Motherboard Name\n");
	uhd_usrp_get_mboard_name(usrp, mboard, mboard_name, 512);
	fprintf(stderr, "	Motherboard name: %s\n", mboard_name);

	//Set Clock Source
	fprintf(stderr, "Setting Clock Source\n");
	uhd_usrp_set_clock_source(usrp, clk_string, mboard);

	// Check Clock Source
	fprintf(stderr, "Checking Clock Source\n");
	uhd_usrp_get_clock_source(usrp, 0, clock_source_out, 512);
	fprintf(stderr, "	Clock Source: %s\n", clock_source_out);

	//Set Time Source
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
	fprintf(stderr, "Setting RX Gain: %f dB...\n", gain);
	uhd_usrp_set_rx_gain(usrp, gain, channel, "");

	// Check Gain
	uhd_usrp_get_rx_gain(usrp, channel, "", &gain);
	fprintf(stderr, "	Actual RX Gain: %f...\n", gain);

	// Set Freq
	fprintf(stderr, "Setting RX frequency: %f MHz...\n", center_freq/1e6);
	uhd_usrp_set_rx_freq(usrp, &tune_request, channel, &tune_result);

	// Check Freq
	uhd_usrp_get_rx_freq(usrp, channel, &freq);
	fprintf(stderr, "	Actual RX frequency: %f MHz...\n", freq/ 1e6);

	// Set RX Bandwidth
	fprintf(stderr, "Setting RX Bandwidth: %f MHz\n", bandwidth/1e6);
	uhd_usrp_set_rx_bandwidth(usrp, bandwidth, channel);

	// Check RX Bandwidth
	fprintf(stderr, "Checking RX Bandwidth...\n");
	uhd_usrp_get_rx_bandwidth(usrp, channel, &bandwidth_out);
	fprintf(stderr, "	Actual RX Bandwidth: %f MHz\n", bandwidth_out / 1e6);

	// Set up streamer
	stream_args.channel_list = &channel;
	uhd_usrp_get_rx_stream(usrp, &stream_args, rx_streamer);

	// Set up buffer
	uhd_rx_streamer_max_num_samps(rx_streamer, &samps_per_buff);
	fprintf(stderr, "Buffer size in samples: %zu\n", samps_per_buff);
	buff = malloc(samps_per_buff * 2 * sizeof(float));
	buffs_ptr = (void**)&buff;

	return EXIT_SUCCESS;
}

//Streaming Function
int *stream_rx_thread(){
	int get_sync = 0;
	int x = 0;
	int counter = 0;

	// Issue stream command
	fprintf(stderr, "Issuing stream command.\n");
	uhd_rx_streamer_issue_stream_cmd(rx_streamer, &stream_cmd);

	uhd_rx_metadata_error_code_t error_code;
	size_t total_purge_samps;
	while(total_purge_samps <= 200000000){
		size_t num_rx_samps = 0;
		uhd_rx_streamer_recv(rx_streamer, buffs_ptr, samps_per_buff, &md, 0.1, false, &num_rx_samps);

		total_purge_samps += num_rx_samps;
	}

	// Streaming Loop
	while (done!=0) {
		size_t num_rx_samps = 0;
		//Recv Packet
		uhd_rx_streamer_recv(rx_streamer, buffs_ptr, samps_per_buff, &md, 0.1, false, &num_rx_samps); // Default: one_packet = false

		//Check for Error Code
		uhd_rx_metadata_error_code(md, &error_code);

		//Handle error Condition
		if(error_code != UHD_RX_METADATA_ERROR_CODE_NONE){
			fprintf(stderr, "Error code 0x%x was returned during streaming. Aborting.\n", error_code);

			int64_t full_secs;
			double frac_secs;
			uhd_rx_metadata_time_spec(md, &full_secs, &frac_secs);
			fprintf(stderr, "Received packet: %zu samples, %.f full secs, %f frac secs\n",
					num_rx_samps,
					difftime(full_secs, (int64_t) 0),
					frac_secs);
			return_code = EXIT_FAILURE;
			done = 0;
		}

		//Handle Data
		int y = 0;
		for(x = 0; x<3832; x+=2){
			rxWaveform[(y+idx)%FRAME_BUFF_SIZE].im = buff[x+1];
			rxWaveform[(y+idx)%FRAME_BUFF_SIZE].re = buff[x];
			y++;
		}

		idx = (idx+1916)%FRAME_BUFF_SIZE;

		if(idx > 919680 && get_sync == 0){ //919680  458874
			get_sync = 1;
			printf("Init Offset\n");
			sem_post(&get_frame_offset);
		}
		else if(idx < ((6584+offset +FRAME_BUFF_SIZE)%FRAME_BUFF_SIZE) && get_sync ==1){
			if(syncronized == 1){
				get_sync = 2;
			}
		}
		else if(idx > ((6584+offset+FRAME_BUFF_SIZE)%FRAME_BUFF_SIZE) && get_sync == 2){
			get_sync = 1;
			counter++;
			printf("Get Offset\n");
			sem_post(&get_frame_offset);
		}
		if(counter > 10000){
			done = 0;
		}
	}
	sem_post(&get_frame_offset);
	return 0;
}

int terminate(){
	int64_t full_secs;
	double frac_secs;


	fprintf(stderr, "Issuing stop stream command.\n");
	uhd_rx_streamer_issue_stream_cmd(rx_streamer, &stream_cmd_term);

	//Display Run Time
	uhd_rx_metadata_time_spec(md, &full_secs, &frac_secs);
	fprintf(stderr, "Execution Time: %.f full secs, %f frac secs\n",
			difftime(full_secs, (int64_t) 0),
			frac_secs);

	if(buff){
		fprintf(stderr, "Freeing buffer.\n");
		free(buff);
	}
	buff = NULL;
	buffs_ptr = NULL;

	fprintf(stderr, "Cleaning up RX streamer.\n");
	uhd_rx_streamer_free(&rx_streamer);

	fprintf(stderr, "Cleaning up RX metadata.\n");
	uhd_rx_metadata_free(&md);


	fprintf(stderr, "Cleaning up USRP.\n");
	if(return_code != EXIT_SUCCESS && usrp != NULL){
		uhd_usrp_last_error(usrp, error_string, 512);
		fprintf(stderr, "USRP reported the following error: %s\n", error_string);
	}
	uhd_usrp_free(&usrp);


	fprintf(stderr, (return_code ? "Failure\n" : "Success\n"));
	return return_code;
}

