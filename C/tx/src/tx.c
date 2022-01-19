/*
 * Copyright 2015 Ettus Research LLC
 * Copyright 2018 Ettus Research, a National Instruments Company
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <uhd.h>
#include "getopt.h"
#include <math.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "timing/tic.h"
#include "timing/toc.h"

#define center_freq 3619200000

void create_tx_args();
void init_usrp_tx();
void stream_tx();
void terminate_tx();

double freq = center_freq;
double rate = 46.08e6;
double gain = 0;
char* device_args = "serial=3164073, num_send_frames = 256, send_frame_size = 7680";
size_t channel = 0;
uint64_t total_num_samps = 0;
int return_code = EXIT_SUCCESS;
char error_string[512];

uhd_tx_metadata_handle md;
uhd_tx_streamer_handle tx_streamer;
uhd_usrp_handle usrp;

size_t samps_per_buff;
float* buff = NULL;
const void** buffs_ptr = NULL;

// Create other necessary structs
uhd_tune_request_t tune_request = {
		.target_freq = center_freq,
		.rf_freq_policy = UHD_TUNE_REQUEST_POLICY_AUTO,
		.dsp_freq_policy = UHD_TUNE_REQUEST_POLICY_AUTO
};
uhd_tune_result_t tune_result;

uhd_stream_args_t stream_args = {
		.cpu_format = "fc32",
		.otw_format = "sc16",
		.args = "",
		.channel_list = &channel,
		.n_channels = 1
};


int main(int argc, char* argv[]){

	create_tx_args();
	init_usrp_tx();
	stream_tx();
	terminate_tx();

	return return_code;
}

void create_tx_args(){
	// Create USRP
	fprintf(stderr, "Creating USRP with args \"%s\"...\n", device_args);
	uhd_usrp_make(&usrp, device_args);

	// Create TX streamer
	uhd_tx_streamer_make(&tx_streamer);

	// Create TX metadata
	uhd_tx_metadata_make(&md, false, 0, 0.1, true, false);
}

void init_usrp_tx(){
	// Set rate
	fprintf(stderr, "Setting TX Rate: %f...\n", rate);
	uhd_usrp_set_tx_rate(usrp, rate, channel);

	// See what rate actually is
	uhd_usrp_get_tx_rate(usrp, channel, &rate);
	fprintf(stderr, "Actual TX Rate: %f...\n\n", rate);

	// Set gain
	fprintf(stderr, "Setting TX Gain: %f db...\n", gain);
	uhd_usrp_set_tx_gain(usrp, gain, 0, "");

	// See what gain actually is
	uhd_usrp_get_tx_gain(usrp, channel, "", &gain);
	fprintf(stderr, "Actual TX Gain: %f...\n", gain);

	// Set frequency
	fprintf(stderr, "Setting TX frequency: %f MHz...\n", freq / 1e6);
	uhd_usrp_set_tx_freq(usrp, &tune_request, channel, &tune_result);

	// See what frequency actually is
	uhd_usrp_get_tx_freq(usrp, channel, &freq);
	fprintf(stderr, "Actual TX frequency: %f MHz...\n", freq / 1e6);

	// Set up streamer
	stream_args.channel_list = &channel;

	uhd_usrp_get_tx_stream(usrp, &stream_args, tx_streamer);


	// Set up buffer
	uhd_tx_streamer_max_num_samps(tx_streamer, &samps_per_buff);

	fprintf(stderr, "Buffer size in samples: %zu\n", samps_per_buff);
	buff = calloc(sizeof(float), samps_per_buff * 2);
	buffs_ptr = (const void**)&buff;
	size_t i = 0;
	for(i = 0; i < (samps_per_buff*2); i+=2){
		buff[i]   = 0.1f;
		buff[i+1] = 0;
	}
}


void stream_tx(){
	uint64_t num_acc_samps = 0;
	size_t num_samps_sent = 0;
	int count = 0;

	//Send Data
	fprintf(stderr, "Send Data\n");
	while(count < 20000000) {
		if (total_num_samps > 0 && num_acc_samps >= total_num_samps) break;

		uhd_tx_streamer_send(tx_streamer, buffs_ptr, samps_per_buff, &md, 0.1, &num_samps_sent);

		num_acc_samps += num_samps_sent;
		fprintf(stderr, "Sent %zu samples\n", num_samps_sent);
		count++;
	}
}


void terminate_tx(){
	free(buff);

	fprintf(stderr, "Cleaning up TX streamer.\n");
	uhd_tx_streamer_free(&tx_streamer);

	fprintf(stderr, "Cleaning up TX metadata.\n");
	uhd_tx_metadata_free(&md);

	fprintf(stderr, "Cleaning up USRP.\n");
	if(return_code != EXIT_SUCCESS && usrp != NULL){
		uhd_usrp_last_error(usrp, error_string, 512);
		fprintf(stderr, "USRP reported the following error: %s\n", error_string);
	}
	uhd_usrp_free(&usrp);

	//free(device_args);

	fprintf(stderr, (return_code ? "Failure\n" : "Success\n"));
}


