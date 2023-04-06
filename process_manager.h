 #include "process_utils.h"
 #include "d_linked_list.h"
 
// Determine whether the currently running process (if any) has completed.
// If so it should be terminated and its memory deallocated before
// subsequent scheduling tasks are performed.
void check_running_process(ListNode** rdy_q_head_ptr, int* rdy_q_len_ptr, short quantum);

// Identify all processes that have been submitted to the system since
// the last cycle occurred and add them to the input queue in the order 
// they appear in the process file. (Arrival time <= simulation time)
void add_submitted_processes(ListNode** in_q_head_ptr, int* in_q_len_ptr, ProcessList** process_list_ptr);

// Move processes from the input queue to the ready queue upon
// successful memory allocation.
void allocate_memory(ListNode** rdy_q_head_ptr, int* rdy_q_len_ptr, char* mem_mng);

// Determine the process (if any) which runs in this cycle. Depending on the 
// scheduling algorithm, this could be the process that was previously running,
// a resumed process that was previously placed back into the ready queue,
// or a READY process which has not previously executed
void determine_running_process(ListNode** rdy_q_head_ptr, int* rdy_q_len_ptr, char* sched);


// Run the simulation of the process manager
void run_simulation(char* sched, char* mem_mng, char* Q, ProcessList* process_list);