#ifndef PROC_UTILS_INCLUDED
#define PROC_UTILS_INCLUDED

#define IMPLEMENTS_REAL_PROCESS_UTILS
#ifdef IMPLEMENTS_REAL_PROCESS_UTILS
#define SHA_LEN 100
#endif

#define MAX_NAME_LEN 9
#define ALLOCATED 1

typedef int boolean;
#define true 1
#define false 0 


typedef struct process Process;
typedef enum { READY = 2, RUNNING = 3, FINISHED = 0, FINISHED_PROCESS = 1, WAITING = -1} State;

struct process {
    unsigned long arrival_time;
    char name[MAX_NAME_LEN];
    unsigned long service_time;
    unsigned long rem_service_time;
    unsigned short memory_req;
    unsigned short mem_address;
    State state;
    boolean has_mem_alloc;
    unsigned long finish_time;
    unsigned long turnaround;
    #ifdef IMPLEMENTS_REAL_PROCESS_UTILS
    int proc_read;
    int proc_write;
    int mngr_read;
    int mngr_write;
    int pid;
    char sha[SHA_LEN];
    #endif

};

typedef struct process_list ProcessList;

// contains the number of processes in processes alongside the array itself
struct process_list {
    Process* processes;
    int num_processes;
};

// retrives the next process in the file list and shifts everything else up the list
Process next_process(ProcessList** process_list_ptr);


// translates a process state from int to string
const char* translate_state(State state);

#endif