#include "d_linked_list.h"
#include "process_utils.h"

#define MAX_INFO_LEN 100
//#define MAX_MEM_DIGITS 4
//#define MAX_REM_TIME_DIGITS 10

typedef struct event Event;
struct event {
    unsigned long sim_time;
    State state;
    char proc_name[MAX_NAME_LEN];
    char info[MAX_INFO_LEN];
};

// Compares two events according to execution transcript specifications
int event_cmp(ListNode* node1, ListNode* node2);

// Calculates and prints average turnaround time, max and average time overhead, and makespan
void calculate_performance_stats(ListNode* fnsh_q_head, int fnsh_q_len, unsigned long simulation_time);

void print_events(ListNode** event_q_head_ptr, int* event_q_len_ptr);

void add_event(ListNode** event_q_head_ptr, int* event_q_len_ptr, unsigned long sim_time, State state, char proc_name[MAX_NAME_LEN], char* info);

// Prints contents of provided process
void print_process(Process* process);
