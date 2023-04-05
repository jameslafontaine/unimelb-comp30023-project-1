#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include "input_parser.h"

#define DEBUG_MODE

// reads the value of each of the command line arguments
char** read_args(int argc, char** argv) {

    char* filepath = NULL;
    char* scheduler = NULL;
    char* mem_mng = NULL;
    char* quantum = NULL;
    int index;
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
	#ifdef DEBUG_MODE
    printf ("filepath = %s, scheduler = %s, mem_mng = %s, quantum = %s\n", filepath, scheduler, mem_mng, quantum);

    for (index = optind; index < argc; index++)
	    printf ("Non-option argument %s\n", argv[index]);
	#endif

    char** argvalues = malloc(sizeof(char*) * NARGS);
	if (argvalues == NULL) {
		fprintf(stderr, "Malloc failure\n");
		exit(EXIT_FAILURE);
	}
	argvalues[FPATH] = filepath;
	argvalues[SCHEDULER] = scheduler;
	argvalues[MEM_MNG] = mem_mng;
	argvalues[QUANTUM] = quantum; // maybe try to store the index of the relevant args in argv rather than pointers to the values
    return argvalues;
}

Process* read_process_file(char* filepath) {

	// Open file
	FILE* fp = fopen(filepath, "r");
	//printf("Attempted to open file \n");
	if (fp == NULL) {
		perror("fopen");
		exit(EXIT_FAILURE);
	}
	// check if the path passed through the command line is relative or absolute
	/*
	if (filepath[0] == '/' || filepath[0] == '\\') {
		// filepath is absolute
	}
	else {
		// filepath is relative
	}
	*/

	// Read processes into memory
	size_t processes_len = 0, processes_size = 2;
	Process* processes = (Process *) malloc(sizeof(Process) * processes_size);
	if (processes == NULL) {
		fprintf(stderr, "Malloc failure\n");
		exit(EXIT_FAILURE);
	}
	int col1, col3, col4;
	char col2[MAX_NAME_LEN];
	while (fscanf(fp, "%d %s %d %d", &col1, col2, &col3, &col4) == 4) {
		//printf("%d %s %d %d\n", col1, col2, col3, col4);
		if (processes_len == processes_size) {
			processes_size *= 2;
			processes = realloc(processes, sizeof(Process) * processes_size);
			//printf("Realloc attempted\n");
			if (processes == NULL) {
				fprintf(stderr, "Realloc failure\n");
				exit(EXIT_FAILURE);
			}
		}
		processes[processes_len].arrival_time = col1;
		strcpy(processes[processes_len].name, col2);
		processes[processes_len].service_time = col3;
		processes[processes_len++].memory_req = col4;
	}

	// Print values
	#ifdef DEBUG_MODE
	for (int i=0; i < processes_len; i++) {
		Process example = processes[i];
		printf("%d %s %d %d\n", example.arrival_time, example.name, example.service_time, example.memory_req);
	}
	#endif
	fclose(fp);

	return processes;
}
	