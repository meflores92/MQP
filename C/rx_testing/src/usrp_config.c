/*
 * usrp_config.c
 *
 *  Created on: Dec 10, 2021
 *      Author: tornado
 */
#include <uhd.h>

#include "getopt.h"

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <complex.h>


#define EXECUTE_OR_GOTO(label, ...) \
		if(__VA_ARGS__){ \
			goto label; \
		}


int init_params(int option, double freq, double rate, double gain, double bandwidth,
		char* device_args, size_t channel, char* filename, size_t n_samples, bool verbose){

	// Create USRP
	uhd_usrp_handle usrp;
	fprintf(stderr, "Creating USRP with args \"%s\"...\n", device_args);
	EXECUTE_OR_GOTO(terminate,
			uhd_usrp_make(&usrp, device_args)
	)

	// Create RX streamer
	uhd_rx_streamer_handle rx_streamer;
	EXECUTE_OR_GOTO(terminate,
			uhd_rx_streamer_make(&rx_streamer)
	)

	// Create RX metadata
	uhd_rx_metadata_handle md;
	EXECUTE_OR_GOTO(terminate,
			uhd_rx_metadata_make(&md)
	)

	// Create other necessary structs
	uhd_tune_request_t tune_request = {
			.target_freq = freq,
			.rf_freq_policy = UHD_TUNE_REQUEST_POLICY_AUTO,
			.dsp_freq_policy = UHD_TUNE_REQUEST_POLICY_AUTO,
	};
	uhd_tune_result_t tune_result;

	uhd_stream_args_t stream_args = {
			.cpu_format = "fc64",
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
	//
	//	uhd_stream_cmd_t stream_cmd = {
	//				.stream_mode = UHD_STREAM_MODE_NUM_SAMPS_AND_DONE,
	//				.num_samps = n_samples,
	//				.stream_now = true
	//		};

	if(false){
		terminate:
		return 1;
	}

	return 0;
}

int configure(){
	//Set Clock Source External
	fprintf(stderr, "Setting Clock Source\n");
	size_t mboard = 0;
	char *clk_string = "external";
	EXECUTE_OR_GOTO(terminate,
			uhd_usrp_set_clock_source(usrp, clk_string,mboard)
	)

	//Set Time Source External
	fprintf(stderr, "Setting Time Source\n");
	char *time_string = "external";
	EXECUTE_OR_GOTO(terminate,
			uhd_usrp_set_time_source(usrp, time_string,mboard)
	)

	//Check Number of Motherboards
	fprintf(stderr, "Checking Number of Motherboards\n");
	size_t num_mboards_out;
	EXECUTE_OR_GOTO(terminate,
			uhd_usrp_get_num_mboards(usrp, &num_mboards_out)
	)
	fprintf(stderr, "	Number of Motherboards: %d\n", (int)num_mboards_out);

	//Check Motherboard Name
	fprintf(stderr, "Checking Motherboard Name\n");
	char mboard_name[512];
	EXECUTE_OR_GOTO(terminate,
			uhd_usrp_get_mboard_name(usrp, mboard, mboard_name, 512)
	)
	fprintf(stderr, "	Motherboard name: %s\n", mboard_name);

	// Check Clock source
	fprintf(stderr, "Checking Clock Source\n");
	char clock_source_out[512];
	EXECUTE_OR_GOTO(terminate,
			uhd_usrp_get_clock_source(usrp, 0, clock_source_out, 512)
	)
	fprintf(stderr, "	Clock Source: %s\n", clock_source_out);

	// Check Time source
	fprintf(stderr, "Checking Time Source\n");
	char time_source_out[512];
	EXECUTE_OR_GOTO(terminate,
			uhd_usrp_get_time_source(usrp, 0, time_source_out, 512)
	)
	fprintf(stderr, "	Time Source: %s\n", time_source_out);

	// Set rate
	fprintf(stderr, "Setting RX Rate: %f...\n", rate);
	EXECUTE_OR_GOTO(terminate,
			uhd_usrp_set_rx_rate(usrp, rate, channel)
	)

	// See what rate actually is
	EXECUTE_OR_GOTO(terminate,
			uhd_usrp_get_rx_rate(usrp, channel, &rate)
	)
	fprintf(stderr, "	Actual RX Rate: %f...\n", rate);

	// Set gain
	fprintf(stderr, "Setting RX Gain: %f dB...\n", gain);
	EXECUTE_OR_GOTO(terminate,
			uhd_usrp_set_rx_gain(usrp, gain, channel, "")
	)

	// See what gain actually is
	EXECUTE_OR_GOTO(terminate,
			uhd_usrp_get_rx_gain(usrp, channel, "", &gain)
	)
	fprintf(stderr, "	Actual RX Gain: %f...\n", gain);

	// Set frequency
	fprintf(stderr, "Setting RX frequency: %f MHz...\n", freq/1e6);
	EXECUTE_OR_GOTO(terminate,
			uhd_usrp_set_rx_freq(usrp, &tune_request, channel, &tune_result)
	)

	// See what frequency actually is
	EXECUTE_OR_GOTO(terminate,
			uhd_usrp_get_rx_freq(usrp, channel, &freq)
	)
	fprintf(stderr, "	Actual RX frequency: %f MHz...\n", freq / 1e6);

	// Set RX Bandwidth
	fprintf(stderr, "Setting RX Bandwidth: %f MHz\n", bandwidth/1e6);
	EXECUTE_OR_GOTO(terminate,
			uhd_usrp_set_rx_bandwidth(usrp, bandwidth, channel)
	)

	// See what RX Bandwidth is
	fprintf(stderr, "Checking RX Bandwidth...\n");
	double bandwidth_out;
	EXECUTE_OR_GOTO(terminate,
			uhd_usrp_get_rx_bandwidth(usrp, channel, &bandwidth_out)
	)
	fprintf(stderr, "	Actual RX Bandwidth: %f MHz\n", bandwidth_out / 1e6);

	// Set up streamer
	stream_args.channel_list = &channel;
	EXECUTE_OR_GOTO(terminate,
			uhd_usrp_get_rx_stream(usrp, &stream_args, rx_streamer)
	)

	// Set up buffer
	EXECUTE_OR_GOTO(terminate,
			uhd_rx_streamer_max_num_samps(rx_streamer, &samps_per_buff)
	)
	fprintf(stderr, "Buffer size in samples: %zu\n", samps_per_buff);
	buff = malloc(samps_per_buff * 2 * sizeof(double));
	buffs_ptr = (void**)&buff;

	// Issue stream command
	fprintf(stderr, "Issuing stream command.\n");
	EXECUTE_OR_GOTO(terminate,
			uhd_rx_streamer_issue_stream_cmd(rx_streamer, &stream_cmd)
	)

	if(false){
		terminate:
		return 1;
	}

	return 0;
}

