#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "process_utils.h"

Process next_process(ProcessList** process_list_ptr) {
    ProcessList* process_list = *process_list_ptr;
    int num_processes = process_list->num_processes;

    Process next_process = (process_list->processes)[0];

    // left shift all processes in the processes list by 1 position
    for (int i=1; i < num_processes; i++) {
        (process_list->processes)[i-1] = (process_list->processes)[i];
    }        
    process_list->num_processes -= 1;
    process_list->processes = (Process *) realloc(process_list->processes, sizeof(Process) * num_processes);
    if (process_list->processes == NULL) {
			fprintf(stderr, "Realloc failure\n");
			exit(EXIT_FAILURE);
		}
        
    return next_process;
}


// translates a process state from int to string
const char* translate_state(State state) {
    if (state == READY) {
        return "READY";
    }
    else if (state == RUNNING) {
        return "RUNNING";
    }
    else if (state == FINISHED) {
        return "FINISHED";
    }
    else if (state == FINISHED_PROCESS) {
        return "FINISHED-PROCESS";
    }
}