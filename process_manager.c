#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "process_manager.h"



#define EQUAL 0

void run_simulation(char* sched, char* mem_mng, char* Q, ProcessList* process_list) {
    unsigned long simulation_time = 0;
    //char* scheduler = sched;
    //char* mem_mng = mm;
    short quantum = (short) *Q;

    ListNode* in_queue_head = NULL;
    int in_queue_len = 0;

    ListNode* rdy_queue_head = NULL;
    int rdy_queue_len = 0;

    Process* running_proc = NULL;

    ListNode* fnsh_queue_head = NULL;
    int fnsh_queue_len = 0;

    ListNode* memory_list = NULL;

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
    while (process_list->num_processes > 0 || in_queue_len > 0 || rdy_queue_len > 0 || running_proc != NULL) {
        check_running_process(&running_proc, &rdy_queue_head, &rdy_queue_len, &fnsh_queue_head, &fnsh_queue_len, simulation_time, quantum, sched, &memory_list, mem_mng);
        add_submitted_processes(&in_queue_head, &in_queue_len, &process_list, simulation_time);
        allocate_memory(&in_queue_head, &in_queue_len, &rdy_queue_head, &rdy_queue_len, &memory_list, mem_mng);
        determine_running_process(&running_proc, &rdy_queue_head, &rdy_queue_len, sched); 
        // Process manager sleeps for quantum seconds, after which a new cycle begins
        simulation_time += quantum;
    }
    free_list(in_queue_head);
    free_list(rdy_queue_head);
    free(running_proc);
    free_list(fnsh_queue_head);
    free_list(memory_list);


}

// Determine whether the currently running process (if any) has completed.
// If so it should be terminated and its memory deallocated before
// subsequent scheduling tasks are performed.
void check_running_process(Process** running_proc_ptr, ListNode** rdy_q_head_ptr, int* rdy_q_len_ptr, ListNode** fnsh_q_head_ptr, int* fnsh_q_len_ptr, unsigned long sim_time, 
short quantum, char* sched, ListNode** mem_list_ptr, char* mem_mng) {

    
    // do nothing if there is no currently running process
    if (*running_proc_ptr = NULL) {
        return;
    } 
    else {
        // subtract quantum from the service time of the currently running process
        (*running_proc_ptr)->service_time -= quantum;
        Process* running_process = *running_proc_ptr;
        // check if running process' service time has elapsed and is therefore finished
        if (running_process->service_time <= 0) {
            deallocate_memory(running_proc_ptr, mem_list_ptr, mem_mng);
            (*running_proc_ptr)->state = FINISHED;
            *fnsh_q_head_ptr = enqueue(*fnsh_q_head_ptr, *running_proc_ptr);
            *fnsh_q_len_ptr += 1;
            free(*running_proc_ptr);
            *running_proc_ptr = NULL;
        }
        // if we are using round robin scheduling then unless this is the only ready process, we must preempt it
        else if (strcmp(sched, "RR") == EQUAL && *rdy_q_len_ptr > 0) {
            (*running_proc_ptr)->state = READY;
            *rdy_q_head_ptr = enqueue(*rdy_q_head_ptr, *running_proc_ptr);
            *rdy_q_len_ptr += 1;
            free(*running_proc_ptr);
            *running_proc_ptr = NULL;
        }
        return;
    }
}

// Identify all processes that have been submitted to the system since
// the last cycle occurred and add them to the input queue in the order 
// they appear in the process file. (Arrival time <= simulation time)
void add_submitted_processes(ListNode** in_q_head_ptr, int* in_q_len_ptr, ProcessList** process_list_ptr, unsigned long sim_time) {
    Process next_proc;
    while ((*process_list_ptr)->num_processes > 0 && ((*process_list_ptr)->processes)[0].arrival_time <= sim_time) {
        next_proc = next_process(process_list_ptr);
        *in_q_head_ptr = enqueue(*in_q_head_ptr , &next_proc);
        *in_q_len_ptr += 1;
    }

    return;

}

// The process manager will use ONE of the following methods to allocate memory to processes:
// -- Assume memory requirements are always immediately satisfied (e.g. there is an
//    infinite amount of memory). All the arrived processes will automatically enter a 
//    READY state, and be placed at the end of the ready queue in the same order as they appear in
//    the input queue OR
//
// -- Allocate memory to processes in the input queue according to a memory allocation algorithm.
//    Processes for which memory allocation is successful enter a READY state, and are placed at the end of
//    the ready queue in order of memory allocation to await CPU time.
void allocate_memory(ListNode** in_q_head_ptr, int* in_q_len_ptr, ListNode** rdy_q_head_ptr, int* rdy_q_len_ptr, 
                     ListNode** mem_list, char* mem_mng) {

    Process next_proc;
    // if the memory allocation method is infinite then all processes in the input queue will automatically enter
    // the ready queue in the same order as they appear in the input queue
    if (strcmp(mem_mng, "infinite") == EQUAL) {
        
        while (*in_q_len_ptr > 0) {
            next_proc = *((Process *)(*in_q_head_ptr)->element);
            next_proc.state = READY;
            *in_q_head_ptr = dequeue(in_q_head_ptr);
            *in_q_len_ptr -= 1;
            *rdy_q_head_ptr = enqueue(*in_q_head_ptr , &next_proc);
            *rdy_q_len_ptr += 1;
            
        }
        
    }
    // if the memory allocation method is best-fit then processes in the input queue will enter
    // the ready queue if memory is successfully allocated to them via best fit
    else if (strcmp(mem_mng, "best-fit") == EQUAL) {

    }
    else {
        fprintf(stderr, "Invalid memory allocation method\n");
		exit(EXIT_FAILURE);
    }


}
// Deallocate memory for a finished process
void deallocate_memory(Process** running_proc_ptr, ListNode** mem_list, char* mem_mng) {
    Process finished_proc;
    // if the memory allocation method is infinite then no memory needs to be deallocated
    if (strcmp(mem_mng, "infinite") == EQUAL) {
        return;
    }
    // if the memory allocation method is best-fit then move the finished process to the finished queue 
    // and deallocate memory by checking if holes need to be joined and updating the memory linked list
    else if (strcmp(mem_mng, "best-fit") == EQUAL) {

    }
    else {
        fprintf(stderr, "Invalid memory allocation method\n");
		exit(EXIT_FAILURE);
    }
}

// Determine the process (if any) which runs in this cycle. Depending on the 
// scheduling algorithm, this could be the process that was previously running,
// a resumed process that was previously placed back into the ready queue,
// or a READY process which has not previously executed
void determine_running_process(Process** running_proc_ptr, ListNode** rdy_q_head_ptr, int* rdy_q_len_ptr, char* sched) {
    // if the scheduling method is shortest job first then the process with the smallest service time gets
    // to run until completion, so we only need to let a new process run when there isn't a current running process
    if (strcmp(sched, "SJF") == EQUAL && *running_proc_ptr == NULL && *rdy_q_len_ptr > 0) {
        
        // find the process in the ready queue with the smallest service time, choose earliest arrival time then
        // the process that comes first lexicographically in the case of ties

        unsigned long min_service_time = __UINT32_MAX__;
        unsigned long min_arrival_time = __UINT32_MAX__;
        char min_process_name[MAX_NAME_LEN] = "~~~~~~~~";

        *running_proc_ptr = malloc(sizeof(Process));
        if (*running_proc_ptr == NULL) {
		    fprintf(stderr, "Malloc failure\n");
		    exit(EXIT_FAILURE);
	    }

        
        //**running_proc_ptr = selected_process;
        //(*running_proc_ptr)->state = RUNNING; 
    }
    // if the scheduling method is round robin then simply let the next process in the ready queue run
    else if (strcmp(sched, "RR") == EQUAL && *rdy_q_len_ptr > 0) {

        *running_proc_ptr = malloc(sizeof(Process));
        if (*running_proc_ptr == NULL) {
		    fprintf(stderr, "Malloc failure\n");
		    exit(EXIT_FAILURE);
	    }
        Process next_proc = *((Process *)(*rdy_q_head_ptr)->element);
        *rdy_q_head_ptr = dequeue(rdy_q_head_ptr);
        *rdy_q_len_ptr -= 1;
        **running_proc_ptr = next_proc;
        (*running_proc_ptr)->state = RUNNING;        
    }
    else {
        fprintf(stderr, "Invalid scheduling method\n");
		exit(EXIT_FAILURE);
    }
}

ListNode* enqueue(ListNode* queue_head, Process* process_ptr) {
    return insert_at_tail(queue_head, process_ptr);
}

ListNode* dequeue(ListNode* queue_head) {
    return remove_head(queue_head);
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