#include <assert.h>
#include <getopt.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "input_parser.h"

#define DEBUG_MODE // comment this line out when you don't want debugging code
				   // to run

#ifdef DEBUG_MODE
// debug code
#endif

int main(int argc, char** argv) {
	char** argvalues = read_args(argc, argv);
	Process* processes = read_process_file(argvalues[FPATH]);



	free(argvalues);
	argvalues = NULL;
	free(processes);
	processes = NULL;
	
	return 0;
}