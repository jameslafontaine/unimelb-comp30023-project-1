#include <assert.h>
#include <getopt.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "input_parser.h"
#include "process_manager.h"

//#define DEBUG_MODE

int main(int argc, char** argv) {
	char** argvalues = read_args(argc, argv);
	ProcessList* process_list = read_process_file(argvalues[FPATH]);
	//printf("%s %s %s\n", argvalues[SCHEDULER], argvalues[MEM_MNG], argvalues[QUANTUM]);
	run_simulation(argvalues[SCHEDULER], argvalues[MEM_MNG], argvalues[QUANTUM], process_list);

	free(argvalues);
	argvalues = NULL;
	free(process_list->processes);
	free(process_list);
	process_list = NULL;

	return 0;
}