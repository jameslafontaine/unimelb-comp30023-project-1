#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include "input_parser.h"

/* Reads in the specified argument values and returns pointers to those arguments */
char** read_args(int argc, char** argv) {

    char* filepath = NULL;
    char* scheduler = NULL;
    char* mem_mng = NULL;
    char* quantum = NULL;
    int c;

    opterr = 0;

    while ((c = getopt(argc, argv, "f:s:m:q:")) != -1)
	    switch (c)
	    {
	    case 'f':
		    filepath = optarg;
		    break;
	    case 's':
		    scheduler = optarg;
		    break;
	    case 'm':
		    mem_mng = optarg;
		    break;
        case 'q':
		    quantum = optarg;
		    break;
	    case '?':
		    if (optopt == 'f' || optopt == 's' || optopt == 'm' || optopt == 'q')
		        fprintf (stderr, "Option -%c requires an argument.\n", optopt);
		    else if (isprint(optopt))
		        fprintf (stderr, "Unknown option `-%c'.\n", optopt);
		    else
		        fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
		    exit(EXIT_FAILURE);
	    default:
		    abort ();
	  }

    char** argvalues = malloc(sizeof(char*) * NARGS);
	if (argvalues == NULL) {
		fprintf(stderr, "Malloc failure\n");
		exit(EXIT_FAILURE);
	}
	if (filepath == NULL) {
		fprintf(stderr, "Error - No filepath provided\n");
		exit(EXIT_FAILURE);
	}
	else if (scheduler == NULL) {
		fprintf(stderr, "Error - No scheduling method provided\n");
		exit(EXIT_FAILURE);
	}
	else if (mem_mng == NULL) {
		fprintf(stderr, "Error - No memory management method provided\n");
		exit(EXIT_FAILURE);
	}
	else if (quantum == NULL) {
		fprintf(stderr, "Error - No quantum provided\n");
		exit(EXIT_FAILURE);
	}
	argvalues[FPATH] = filepath;
	argvalues[SCHEDULER] = scheduler;
	argvalues[MEM_MNG] = mem_mng;
	argvalues[QUANTUM] = quantum;
    return argvalues;
}


/* Reads the process file located via the provided absolute or relative filepath and returns
	an array of those processes in the form of struct processes defined in process_utils.h */
ProcessList* read_process_file(char* filepath) {

	// Open file
	FILE* fp = fopen(filepath, "r");
	if (fp == NULL) {
		perror("fopen");
		exit(EXIT_FAILURE);
	}

	// Read processes into memory
	size_t processes_len = 0, processes_size = 2;
	Process* processes = (Process *) malloc(sizeof(Process) * processes_size);
	if (processes == NULL) {
		fprintf(stderr, "Malloc failure\n");
		exit(EXIT_FAILURE);
	}
	unsigned long col1, col3;
	unsigned short col4;
	char col2[MAX_NAME_LEN];
	while (fscanf(fp, "%lu %s %lu %hd", &col1, col2, &col3, &col4) == 4) {
		if (processes_len == processes_size) {
			processes_size *= 2;
			processes = realloc(processes, sizeof(Process) * processes_size);
			if (processes == NULL) {
				fprintf(stderr, "Realloc failure\n");
				exit(EXIT_FAILURE);
			}
		}
		processes[processes_len].arrival_time = col1;
		strcpy(processes[processes_len].name, col2);
		processes[processes_len].service_time = col3;
		processes[processes_len].rem_service_time = col3;
		processes[processes_len++].memory_req = col4;
	}

	fclose(fp);

	ProcessList* process_list = (ProcessList *) malloc(sizeof(ProcessList));
	if (process_list == NULL) {
		fprintf(stderr, "Malloc failure\n");
		exit(EXIT_FAILURE);
	}
	process_list->num_processes = processes_len;
	process_list->processes = processes;

	return process_list;
}
	