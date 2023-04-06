#include <assert.h>
#include <getopt.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "input_parser.h"
#include "process_manager.h"

#define DEBUG_MODE // comment this line out when you don't want debugging code
				   // to run

#ifdef DEBUG_MODE
// debug code
#endif

int main(int argc, char** argv) {
	char** argvalues = read_args(argc, argv);
	ProcessList* process_list = read_process_file(argvalues[FPATH]);
	begin_simulation(argvalues[SCHEDULER], argvalues[MEM_MNG], argvalues[QUANTUM], process_list);

	free(argvalues);
	argvalues = NULL;
	free(process_list);
	processes = NULL;

	return 0;
}