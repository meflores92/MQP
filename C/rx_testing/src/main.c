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



	size_t samps_per_buff;
	double complex *buff = NULL;
	void **buffs_ptr = NULL;
	FILE *fp = NULL;
	size_t num_acc_samps = 0;


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
		fwrite(buff, sizeof(double) * 2, num_rx_samps, fp);
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

