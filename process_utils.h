
#define MAX_NAME_LEN 9
#define ALLOCATED

typedef int bool;
#define true 1
#define false 0

typedef struct process Process;
typedef enum { READY = 1, RUNNING = 2, FINISHED = 0} State;

struct process {
    int arrival_time;
    char name[MAX_NAME_LEN];
    int service_time;
    int memory_req;
    State state;
    bool mem_alloc;
};
