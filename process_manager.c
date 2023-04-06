#include <stdlib.h>
#include "process_manager.h"

void run_simulation(char* sched, char* mem_mng, char* Q, ProcessList* process_list) {
    unsigned long simulation_time = 0;
    //char* scheduler = sched;
    //char* mem_mng = mm;
    short quantum = (short) *Q;

    ListNode* in_queue_head = NULL;
    int in_queue_len = 0;

    ListNode* rdy_queue_head = NULL;
    int rdy_queue_len = 0;

    // first process will always be submitted instantly (as arrival time is 0), add it and any other processes
    // with arrival time 0 to the input queue and begin allocating memory and running one of
    // them
    /*
    add_submitted_processes(&in_queue_head, &in_queue_len, process_list);
    allocate_memory(&rdy_queue_head, &rdy_queue_len);
    determine_running_process(&rdy_queue_head, &rdy_queue_len);   
    simulation_time += quantum;
    */
    // complete cycles while there are still processes that haven't finished execution
    while (process_list->num_processes > 0 || in_queue_len > 0 || rdy_queue_len > 0) {
        check_running_process(&rdy_queue_head, &rdy_queue_len, quantum);
        add_submitted_processes(&in_queue_head, &in_queue_len, &process_list);
        allocate_memory(&rdy_queue_head, &rdy_queue_len, mem_mng);
        determine_running_process(&rdy_queue_head, &rdy_queue_len, sched); 
        // Process manager sleeps for quantum seconds, after which a new cycle begins
        simulation_time += quantum;
    }


}

void check_running_process(ListNode** rdy_q_head_ptr, int* rdy_q_len_ptr, short quantum) {
 
}

void add_submitted_processes(ListNode** in_q_head_ptr, int* in_q_len_ptr, ProcessList** process_list_ptr) {

}
/*
void complete_cycle(char* scheduler, char* mm, short quantum, ProcessList* process_list) {

    // Determine whether the currently running process (if any) has completed.
    // If so it should be terminated and its memory deallocated before
    // subsequent scheduling tasks are performed.
    

    // Identify all processes that have been submitted to the system since
    // the last cycle occurred and add them to the input queue in the order 
    // they appear in the process file. (Arrival time <= simulation time)
    

    // Move processes from the input queue to the ready queue upon
    // successful memory allocation.
    

    // Determine the process (if any) which runs in this cycle. Depending on the 
    // scheduling algorithm, this could be the process that was previously running,
    // a resumed process that was previously placed back into the ready queue,
    // or a READY process which has not previously executed
    
}
*/

// The process manager will use ONE of the following methods to allocate memory to processes:
// -- Assume memory requirements are always immediately satisfied (e.g.m there is an
//    infinite amount of memory). All the arrived processes will automatically enter a 
//    READY state, and be placed at the end of the ready queue in the same order as they appear in
//    the input queue OR
//
// -- Allocate memory to processes in the input queue according to a memory allocation algorithm.
//    Processes for which memory allocation is successful enter a READY state, and are placed at the end of
//    the ready queue in order of memory allocation to await CPU time.
void allocate_memory(ListNode** rdy_q_head_ptr, int* rdy_q_len_ptr, char* mem_mng) {

}

void determine_running_process(ListNode** rdy_q_head_ptr, int* rdy_q_len_ptr, char* sched) {
    
}