#include "process_utils.h"
#include "d_linked_list.h"


#define MAX_MEM 2048

// Compares two processes according to SJF specifications
int sjf_cmp(ListNode** node1, ListNode** node2);

// Run the simulation of the process manager
void run_simulation(char* sched, char* mem_mng, char* Q, ProcessList* process_list);


// Determine whether the currently running process (if any) has completed.
// If so it should be terminated and its memory deallocated before
// subsequent scheduling tasks are performed.
void check_running_process(Process** running_proc_ptr, ListNode** rdy_q_head_ptr, int* rdy_q_len_ptr, 
ListNode** fnsh_q_head_ptr, int* fnsh_q_len_ptr, unsigned long sim_time, short quantum, char* sched, ListNode** mem_list_ptr, 
char* mem_mng, ListNode** event_q_head_ptr, int* event_q_len_ptr);

// Identify all processes that have been submitted to the system since
// the last cycle occurred and add them to the input queue in the order 
// they appear in the process file. (Arrival time <= simulation time)
void add_submitted_processes(ListNode** in_q_head_ptr, int* in_q_len_ptr, ProcessList** process_list_ptr, unsigned long sim_time, ListNode** event_q_head_ptr, int* event_q_len_ptr);

// Move processes from the input queue to the ready queue upon
// successful memory allocation
void allocate_memory(ListNode** in_q_head_ptr, int* in_q_len_ptr, ListNode** rdy_q_head_ptr, int* rdy_q_len_ptr, ListNode** mem_list_ptr, char* mem_mng, unsigned long sim_time, ListNode** event_q_head_ptr, int* event_q_len_ptr);

// Remove processes from the ready queue once their service time has elapsed 
// and deallocate memory
void deallocate_memory(Process** running_proc_ptr, ListNode** mem_list_ptr, char* mem_mng);

// Determine the process (if any) which runs in this cycle. Depending on the 
// scheduling algorithm, this could be the process that was previously running,
// a resumed process that was previously placed back into the ready queue,
// or a READY process which has not previously executed
void schedule_process(Process** running_proc_ptr, ListNode** rdy_q_head_ptr, int* rdy_q_len_ptr, char* sched, unsigned long sim_time, ListNode** event_q_head_ptr, int* event_q_len_ptr);

// Performs the necessary steps to begin running the next process in the ready queue
void start_running(Process** running_proc_ptr, ListNode** rdy_q_head_ptr, int* rdy_q_len_ptr);

