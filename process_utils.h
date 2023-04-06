
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
    State state;
    boolean has_mem_alloc;
};

typedef struct process_list ProcessList;

struct process_list {
    Process* processes;
    int num_processes;
};

// moves the next process in the file list to the input queue
Process next_process(ProcessList** process_list_ptr);