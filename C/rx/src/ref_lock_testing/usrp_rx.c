/*
 ============================================================================
 Name        : rx_testing
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : Receive signal with USRP and save to file for post processing in MATLAB
 ============================================================================
 */

#include <uhd.h>

#include "getopt.h"

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <complex.h>

#include "cpp/ref_lock.h"

#define EXECUTE_OR_GOTO(label, ...) \
		if(__VA_ARGS__){ \
			return_code = EXIT_FAILURE; \
			goto label; \
		}

bool stop_signal_called = false;

void sigint_handler(int code){
	(void)code;
	stop_signal_called = true;
}

//Function Declaration
//int check_ref_locked(uhd_usrp_handle usrp, size_t mboard);

int main(int argc, char* argv[]) {
	int option = 0;
	double freq = 3619200000;
	double rate = 46.08e6;
	double gain = 70.0;
	double bandwidth = 40.00e6; //Default for B210 == 56.00Mhz
	char* device_args = "serial=3164073, num_recv_frames = 512, recv_frame_size = 7680"; //originally null
	size_t channel = 0;
	char* filename = "/home/tornado/Desktop/rx_testing/out.dat";
	size_t n_samples = 40080000;
	bool verbose = true;
	int return_code = EXIT_SUCCESS;
	bool custom_filename = false;
	char error_string[512];

	// Process options
	while((option = getopt(argc, argv, "a:f:r:g:n:o:vh")) != -1){
		switch(option){
		case 'a':
			device_args = strdup(optarg);
			break;

		case 'f':
			freq = atof(optarg);
			break;

		case 'r':
			rate = atof(optarg);
			break;

		case 'g':
			gain = atof(optarg);
			break;

		case 'n':
			n_samples = atoi(optarg);
			break;

		case 'o':
			filename = strdup(optarg);
			custom_filename = true;
			break;

		case 'v':
			verbose = true;
			break;

		case 'h':
			//print_help();
			goto free_option_strings;

		default:
			//print_help();
			return_code = EXIT_FAILURE;
			goto free_option_strings;
		}
	}

	if (!device_args)
		device_args = strdup("");

	// Create USRP
	uhd_usrp_handle usrp;
	fprintf(stderr, "Creating USRP with args \"%s\"...\n", device_args);
	EXECUTE_OR_GOTO(free_option_strings,
			uhd_usrp_make(&usrp, device_args)
	)

	// Create RX streamer
	uhd_rx_streamer_handle rx_streamer;
	EXECUTE_OR_GOTO(free_usrp,
			uhd_rx_streamer_make(&rx_streamer)
	)

	// Create RX metadata
	uhd_rx_metadata_handle md;
	EXECUTE_OR_GOTO(free_rx_streamer,
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


	size_t samps_per_buff;
	double complex *buff = NULL;
	void **buffs_ptr = NULL;
	FILE *fp = NULL;
	size_t num_acc_samps = 0;

	size_t mboard = 0;
	//Check Number of Motherboards
	fprintf(stderr, "Checking Number of Motherboards\n");
	size_t num_mboards_out;
	EXECUTE_OR_GOTO(free_rx_metadata,
			uhd_usrp_get_num_mboards(usrp, &num_mboards_out)
	)
	fprintf(stderr, "	Number of Motherboards: %d\n", (int)num_mboards_out);

	//Check Motherboard Name
	fprintf(stderr, "Checking Motherboard Name\n");
	char mboard_name[512];
	EXECUTE_OR_GOTO(free_rx_metadata,
			uhd_usrp_get_mboard_name(usrp, mboard, mboard_name, 512)
	)
	fprintf(stderr, "	Motherboard name: %s\n", mboard_name);

	//Set Clock Source External
	fprintf(stderr, "Setting Clock Source\n");
	char *clk_string = "external";
	EXECUTE_OR_GOTO(free_rx_metadata,
			uhd_usrp_set_clock_source(usrp, clk_string, mboard)
	)

	//Set Time Source External
	fprintf(stderr, "Setting Time Source\n");
	char *time_string = "external";
	EXECUTE_OR_GOTO(free_rx_metadata,
			uhd_usrp_set_time_source(usrp, time_string, mboard)
	)

	// Check Clock source
	fprintf(stderr, "Checking Clock Source\n");
	char clock_source_out[512];
	EXECUTE_OR_GOTO(free_rx_metadata,
			uhd_usrp_get_clock_source(usrp, 0, clock_source_out, 512)
	)
	fprintf(stderr, "	Clock Source: %s\n", clock_source_out);

	// Check Time source
	fprintf(stderr, "Checking Time Source\n");
	char time_source_out[512];
	EXECUTE_OR_GOTO(free_rx_metadata,
			uhd_usrp_get_time_source(usrp, 0, time_source_out, 512)
	)
	fprintf(stderr, "	Time Source: %s\n", time_source_out);


	check_ref_locked(usrp, mboard);

//	//Check to see if External Reference Locked
//	if (check_ref_locked(usrp, mboard)) {
//		fprintf(stderr, "USRP locked to external reference!\n");
//	} else {
//		fprintf(stderr, "Failed to lock to external reference. Exiting.\n");
//		exit(EXIT_FAILURE);
//	}

	// Set rate
	fprintf(stderr, "Setting RX Rate: %f...\n", rate);
	EXECUTE_OR_GOTO(free_rx_metadata,
			uhd_usrp_set_rx_rate(usrp, rate, channel)
	)

	// See what rate actually is
	EXECUTE_OR_GOTO(free_rx_metadata,
			uhd_usrp_get_rx_rate(usrp, channel, &rate)
	)
	fprintf(stderr, "	Actual RX Rate: %f...\n", rate);

	// Set gain
	fprintf(stderr, "Setting RX Gain: %f dB...\n", gain);
	EXECUTE_OR_GOTO(free_rx_metadata,
			uhd_usrp_set_rx_gain(usrp, gain, channel, "")
	)

	// See what gain actually is
	EXECUTE_OR_GOTO(free_rx_metadata,
			uhd_usrp_get_rx_gain(usrp, channel, "", &gain)
	)
	fprintf(stderr, "	Actual RX Gain: %f...\n", gain);

	// Set frequency
	fprintf(stderr, "Setting RX frequency: %f MHz...\n", freq/1e6);
	EXECUTE_OR_GOTO(free_rx_metadata,
			uhd_usrp_set_rx_freq(usrp, &tune_request, channel, &tune_result)
	)

	// See what frequency actually is
	EXECUTE_OR_GOTO(free_rx_metadata,
			uhd_usrp_get_rx_freq(usrp, channel, &freq)
	)
	fprintf(stderr, "	Actual RX frequency: %f MHz...\n", freq / 1e6);

	// Set RX Bandwidth
	fprintf(stderr, "Setting RX Bandwidth: %f MHz\n", bandwidth/1e6);
	EXECUTE_OR_GOTO(free_rx_metadata,
			uhd_usrp_set_rx_bandwidth(usrp, bandwidth, channel)
	)

	// See what RX Bandwidth is
	fprintf(stderr, "Checking RX Bandwidth...\n");
	double bandwidth_out;
	EXECUTE_OR_GOTO(free_rx_metadata,
			uhd_usrp_get_rx_bandwidth(usrp, channel, &bandwidth_out)
	)
	fprintf(stderr, "	Actual RX Bandwidth: %f MHz\n", bandwidth_out / 1e6);

	// Set up streamer
	stream_args.channel_list = &channel;
	EXECUTE_OR_GOTO(free_rx_streamer,
			uhd_usrp_get_rx_stream(usrp, &stream_args, rx_streamer)
	)

	// Set up buffer
	EXECUTE_OR_GOTO(free_rx_streamer,
			uhd_rx_streamer_max_num_samps(rx_streamer, &samps_per_buff)
	)
	fprintf(stderr, "Buffer size in samples: %zu\n", samps_per_buff);
	buff = malloc(samps_per_buff * 2 * sizeof(double));
	buffs_ptr = (void**)&buff;

	// Issue stream command
	fprintf(stderr, "Issuing stream command.\n");
	EXECUTE_OR_GOTO(free_buffer,
			uhd_rx_streamer_issue_stream_cmd(rx_streamer, &stream_cmd)
	)

	// Set up file output
	fp = fopen(filename, "wb");

	uhd_rx_metadata_error_code_t error_code;
	signal(SIGINT, &sigint_handler);
	fprintf(stderr, "Press Ctrl+C to stop streaming...\n");

	// Actual streaming
	while (num_acc_samps< n_samples) {
		size_t num_rx_samps = 0;
		if (stop_signal_called) break;
		//		EXECUTE_OR_GOTO(close_file,
		//				uhd_rx_streamer_recv(rx_streamer, buffs_ptr, samps_per_buff, &md, 3.0, false, &num_rx_samps)
		//		)

		uhd_rx_streamer_recv(rx_streamer, buffs_ptr, samps_per_buff, &md, 0.1, false, &num_rx_samps); // Default: one_packet = false

		uhd_rx_metadata_error_code(md, &error_code);

		//		EXECUTE_OR_GOTO(close_file,
		//				uhd_rx_metadata_error_code(md, &error_code)
		//		)
		if(error_code != UHD_RX_METADATA_ERROR_CODE_NONE){
			fprintf(stderr, "Error code 0x%x was returned during streaming. Aborting.\n", error_code);

			int64_t full_secs;
			double frac_secs;
			uhd_rx_metadata_time_spec(md, &full_secs, &frac_secs);
			fprintf(stderr, "Received packet: %zu samples, %.f full secs, %f frac secs\n",
					num_rx_samps,
					difftime(full_secs, (int64_t) 0),
					frac_secs);
			goto close_file;
		}

		// Handle data
		//fwrite(buff, sizeof(double) * 2, num_rx_samps, fp);
		//printf("Data 0: %f, Data 1: %f\n", buff[0], buff[1]);
		//		if (verbose) {
		//			int64_t full_secs;
		//			double frac_secs;
		//			uhd_rx_metadata_time_spec(md, &full_secs, &frac_secs);
		//			fprintf(stderr, "Received packet: %zu samples, %.f full secs, %f frac secs\n",
		//					num_rx_samps,
		//					difftime(full_secs, (int64_t) 0),
		//					frac_secs);
		//		}

		num_acc_samps += num_rx_samps;
	}

	fprintf(stderr, "Issuing stop stream command.\n");
	EXECUTE_OR_GOTO(free_buffer,
			uhd_rx_streamer_issue_stream_cmd(rx_streamer, &stream_cmd_term)
	)

	printf("First Index of Last Buffer: Real: %f, Imag: %f\n", creal(buff[0]),cimag(buff[0]));
	printf("Second Index of Last Buffer: Real: %f, Imag: %f\n", creal(buff[1]),cimag(buff[1]));

	int64_t full_secs;
	double frac_secs;
	uhd_rx_metadata_time_spec(md, &full_secs, &frac_secs);
	fprintf(stderr, "Execution Time: %.f full secs, %f frac secs\n",
			difftime(full_secs, (int64_t) 0),
			frac_secs);

	// Cleanup
	close_file:
	fclose(fp);

	free_buffer:
	if(buff){
		if(verbose){
			fprintf(stderr, "Freeing buffer.\n");
		}
		free(buff);
	}
	buff = NULL;
	buffs_ptr = NULL;

	free_rx_streamer:
	if(verbose){
		fprintf(stderr, "Cleaning up RX streamer.\n");
	}
	uhd_rx_streamer_free(&rx_streamer);

	free_rx_metadata:
	if(verbose){
		fprintf(stderr, "Cleaning up RX metadata.\n");
	}
	uhd_rx_metadata_free(&md);

	free_usrp:
	if(verbose){
		fprintf(stderr, "Cleaning up USRP.\n");
	}
	if(return_code != EXIT_SUCCESS && usrp != NULL){
		uhd_usrp_last_error(usrp, error_string, 512);
		fprintf(stderr, "USRP reported the following error: %s\n", error_string);
	}
	uhd_usrp_free(&usrp);

	free_option_strings:
	//	if(device_args) {
	//		free(device_args);
	//	}
	if(custom_filename){
		free(filename);
		puts("reached 5");
	}

	fprintf(stderr, (return_code ? "Failure\n" : "Success\n"));
	return return_code;
}


//int check_ref_locked(uhd_usrp_handle usrp, size_t mboard) {
//	fprintf(stderr, "Entering Function...\n");
//	bool ref_locked = false;
//	if(~ref_locked){
//		fprintf(stderr, "Starting Flag Sat\n");
//	}
//	uhd_sensor_value_handle sensor_value;
////	uhd_sensor_value_data_type_t data_type_out = UHD_SENSOR_VALUE_BOOLEAN;
////	uhd_sensor_value_data_type(sensor_value, &data_type_out);
//	fprintf(stderr, "Waiting for reference lock...\n");
//
//	for (int i = 0; i < 30 && (~ref_locked); i++) {
//		//Original C++ ... ref_locked = s->usrp->get_mboard_sensor("ref_locked", mboard).to_bool();
//		fprintf(stderr, "Entering Loop...\n");
//		const char* name = "ref_locked";
//
//		uhd_usrp_get_mboard_sensor(usrp, "ref_locked", mboard, &sensor_value);//Errors Here
//		fprintf(stderr, "Found Sensor\n");
//
//		uhd_sensor_value_to_bool(sensor_value, &ref_locked);
//		fprintf(stderr, "Found Sensor Value\n");
//
//		if (~ref_locked) {
//			fprintf(stderr, "...\n");
//		}
//	}
//
//	if(ref_locked) {
//		fprintf(stderr, "LOCKED\n");
//	}
//	else {
//		fprintf(stderr, "FAILED\n");
//	}
//
//	return ref_locked;
//}

