
#define MAX_NAME_LEN 9
#define ALLOCATED 1

typedef int boolean;
#define true 1
#define false 0


typedef struct process Process;
typedef enum { READY = 1, RUNNING = 2, FINISHED = 0} State;

struct process {
    unsigned long arrival_time;
    char name[MAX_NAME_LEN];
    unsigned long service_time;
    short memory_req;
    short mem_address;
    State state;
    boolean has_mem_alloc;
};

typedef struct process_list ProcessList;

// contains the number of processes in processes alongside the array itself
struct process_list {
    Process* processes;
    int num_processes;
};

// retrives the next process in the file list and shifts everything else up the list
Process next_process(ProcessList** process_list_ptr);