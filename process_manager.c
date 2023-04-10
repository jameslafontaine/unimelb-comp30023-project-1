#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "process_manager.h"
#include "output.h"

//#define PMDEBUG

void run_simulation(char* sched, char* mem_mng, char* Q, ProcessList* process_list) {
    unsigned long simulation_time = 0;

    short quantum = (short) atoi(Q);

    ListNode* in_queue_head = NULL;
    int in_queue_len = 0;

    ListNode* rdy_queue_head = NULL;
    int rdy_queue_len = 0;

    ListNode* running_head = NULL;

    ListNode* fnsh_queue_head = NULL;
    int fnsh_queue_len = 0;

    ListNode* memory_list_head = NULL;

    ListNode* event_queue_head = NULL;
    int event_queue_len = 0;

    // complete cycles while there are still processes that haven't finished execution
    while (process_list->num_processes > 0 || in_queue_len > 0 || rdy_queue_len > 0 || running_head != NULL) {
        
        #ifdef PMDEBUG
        printf("Simulation time = %lu\n", simulation_time);
        #endif
        check_running_process(&running_head, &in_queue_len, &rdy_queue_head, &rdy_queue_len, &fnsh_queue_head, &fnsh_queue_len, simulation_time, quantum, sched, &memory_list_head, mem_mng, &event_queue_head, &event_queue_len);
        add_submitted_processes(&in_queue_head, &in_queue_len, &process_list, simulation_time, &event_queue_head, &event_queue_len);
        allocate_memory(&in_queue_head, &in_queue_len, &rdy_queue_head, &rdy_queue_len, &memory_list_head, mem_mng, simulation_time, &event_queue_head, &event_queue_len);
        schedule_process(&running_head, &rdy_queue_head, &rdy_queue_len, sched, simulation_time, &event_queue_head, &event_queue_len); 
        print_events(&event_queue_head, &event_queue_len);
        // Process manager sleeps for quantum seconds, after which a new cycle begins
        simulation_time += quantum;
        #ifdef PMDEBUG
        if (simulation_time > 120) {
            exit(EXIT_SUCCESS);
        }
        #endif
    }
    // subtract last quantum added as this cycle doesn't actually occur
    simulation_time -= quantum;
    calculate_performance_stats(fnsh_queue_head, fnsh_queue_len, simulation_time);
    free_list(&in_queue_head);
    free_list(&rdy_queue_head);
    free_list(&running_head);
    free_list(&fnsh_queue_head);
    free_list(&memory_list_head);
    free_list(&event_queue_head);
    in_queue_head = NULL;
    rdy_queue_head = NULL;
    running_head = NULL;
    fnsh_queue_head = NULL;
    memory_list_head = NULL;
    event_queue_head = NULL;
}

// Determine whether the currently running process (if any) has completed.
// If so it should be terminated and its memory deallocated before
// subsequent scheduling tasks are performed.
void check_running_process(ListNode** running_head_ptr, int* in_q_len_ptr, ListNode** rdy_q_head_ptr, int* rdy_q_len_ptr, ListNode** fnsh_q_head_ptr, int* fnsh_q_len_ptr, unsigned long sim_time, 
short quantum, char* sched, ListNode** mem_list_ptr, char* mem_mng, ListNode** event_q_head_ptr, int* event_q_len_ptr) {
    
    #ifdef PMDEBUG
    printf("Checking running processes...\n");
    #endif
    
    // do nothing if there is no currently running process
    if (*running_head_ptr == NULL) {
        return;
    } 
    else {
        Process* running_proc = (Process *)(*running_head_ptr)->element;
        // subtract quantum from the service time of the currently running process, or set
        // the service time to 0 it is less than the quantum to prevent integer overflow
        if (running_proc->rem_service_time < quantum) {
            running_proc->rem_service_time = 0;
        }
        else {
            running_proc->rem_service_time -= quantum;
        }
        #ifdef PMDEBUG
        printf("Remaining service time for current process - %lu\n", running_proc->rem_service_time);
        #endif

        // check if running process' service time has elapsed and is therefore finished
        if (running_proc->rem_service_time == 0) {
            deallocate_memory(running_head_ptr, mem_list_ptr, mem_mng);
            int dummy_len = 0;
            running_proc = transition_process(running_head_ptr, &dummy_len, fnsh_q_head_ptr, fnsh_q_len_ptr, FINISHED);
            running_proc->finish_time = sim_time;
            //print_process(*running_proc_ptr);
            // convert remaining time to string for event printing
            char remaining_time[MAX_INFO_LEN];
            sprintf(remaining_time, "%d", *rdy_q_len_ptr + *in_q_len_ptr);
            add_event(event_q_head_ptr, event_q_len_ptr, sim_time, running_proc->state, running_proc->name, remaining_time);
            //free(*running_proc_ptr);
            //print_process((retrieve_tail(*fnsh_q_head_ptr))->element);
        }
        return;
    }
}

// Identify all processes that have been submitted to the system since
// the last cycle occurred and add them to the input queue in the order 
// they appear in the process file. (Arrival time <= simulation time)
void add_submitted_processes(ListNode** in_q_head_ptr, int* in_q_len_ptr, ProcessList** process_list_ptr, unsigned long sim_time, ListNode** event_q_head_ptr, int* event_q_len_ptr) {
    
    #ifdef PMDEBUG
    printf("Adding submitted processes...\n");
    #endif
    while ((*process_list_ptr)->num_processes > 0 && ((*process_list_ptr)->processes)[0].arrival_time <= sim_time) {
        //printf("Num processes = %d\n", (*process_list_ptr)->num_processes);
        //printf("Next process arrival time = %lu\n", ((*process_list_ptr)->processes)[0].arrival_time);
        //print_list(*in_q_head_ptr);
        Process* next_proc = malloc(sizeof(Process));
        if (next_proc == NULL) {
            fprintf(stderr, "Malloc failure\n");
            exit(EXIT_FAILURE);
        }
        *next_proc = next_process(process_list_ptr);
        *in_q_head_ptr = enqueue(*in_q_head_ptr , next_proc);
        //print_list(*in_q_head_ptr);
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
                     ListNode** mem_list, char* mem_mng, unsigned long sim_time, ListNode** event_q_head_ptr, int* event_q_len_ptr) {
    
    #ifdef PMDEBUG
    printf("Attempting to allocate memory...\n");
    #endif
    // if the memory allocation method is infinite then all processes in the input queue will automatically enter
    // the ready queue in the same order as they appear in the input queue
    if (strcmp(mem_mng, "infinite") == EQUAL) {
        
        while (*in_q_len_ptr > 0) {
            transition_process(in_q_head_ptr, in_q_len_ptr, rdy_q_head_ptr, rdy_q_len_ptr, READY);
        }
        
    }
    // if the memory allocation method is best-fit then processes in the input queue will enter
    // the ready queue if memory is successfully allocated to them via best fit
    else if (strcmp(mem_mng, "best-fit") == EQUAL) {
        int num_processes_checked = 0;
        while (num_processes_checked < *in_q_len_ptr) {
            // see if we can allocate memory to this process using best fit
            if (best_fit_alloc((Process *)((*in_q_head_ptr)->element), mem_list)) {
                Process* next_proc = transition_process(in_q_head_ptr, in_q_len_ptr, rdy_q_head_ptr, rdy_q_len_ptr, READY);
                // convert memory address to string for event printing
                char mem_address[MAX_INFO_LEN];
                sprintf(mem_address, "%hu", next_proc->mem_address);
                add_event(event_q_head_ptr, event_q_len_ptr, sim_time, next_proc->state, next_proc->name, mem_address);        
            }
            // otherwise move this process to the end of the input queue
            else {
                transition_process(in_q_head_ptr, in_q_len_ptr, in_q_head_ptr, in_q_len_ptr, WAITING);
            }
            num_processes_checked += 1;
        }
    }
    else {
        fprintf(stderr, "Invalid memory allocation method\n");
		exit(EXIT_FAILURE);
    }
}

int best_fit_alloc(Process* process, ListNode** mem_list) {
    //unsigned short smallest_hole_size = MAX_MEM + 1;
    //ListNode* smallest_hole = NULL;

    // iterate through memory list until reach end and track smallest hole that is greater
    // than or equal to memory requirement of process
    
    return SUCCESS;
}

void best_fit_dealloc(Process* process, ListNode** mem_list) {
    // remove process from memory list and join holes as needed

    // if listnode to left and / or right of memory listnode used by process is a hole then we
    // join the holes
}



// Deallocate memory for a finished process
void deallocate_memory(ListNode** running_head_ptr, ListNode** mem_list, char* mem_mng) {

    #ifdef PMDEBUG
    printf("Deallocating memory...\n");
    #endif

    // if the memory allocation method is infinite then no memory needs to be deallocated
    if (strcmp(mem_mng, "infinite") == EQUAL) {
        return;
    }
    // if the memory allocation method is best-fit then move the finished process to the finished queue 
    // and deallocate memory by checking if holes need to be joined and updating the memory linked list
    else if (strcmp(mem_mng, "best-fit") == EQUAL) {
        best_fit_dealloc((Process *)(*running_head_ptr)->element, mem_list);
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
void schedule_process(ListNode** running_head_ptr, ListNode** rdy_q_head_ptr, int* rdy_q_len_ptr, char* sched, unsigned long sim_time, ListNode** event_q_head_ptr, int* event_q_len_ptr) {
    
    #ifdef PMDEBUG
    printf("Scheduling process...\n");
    #endif

    // if the scheduling method is shortest job first then the process with the smallest service time gets
    // to run until completion, so we only need to let a new process run when there isn't a current running process
    if (strcmp(sched, "SJF") == EQUAL && *running_head_ptr == NULL && *rdy_q_len_ptr > 0) {
        
        // find the process in the ready queue with the smallest service time, choose earliest arrival time then
        // the process that comes first lexicographically in the case of ties
        static int (*cmp_func)(ListNode*, ListNode*);
        cmp_func = sjf_cmp;
        //print_list(*rdy_q_head_ptr);
        *rdy_q_head_ptr = ins_sort_list(*rdy_q_head_ptr, cmp_func);
        //print_list(*rdy_q_head_ptr);
        run_process(running_head_ptr, rdy_q_head_ptr, rdy_q_len_ptr, event_q_head_ptr, event_q_len_ptr, sim_time);
    }
    // if the scheduling method is round robin then simply let the next process in the ready queue run if
    // there is one by preempting the currently running process, otherwise give more CPU time to the currently running process
    else if (strcmp(sched, "RR") == EQUAL && *rdy_q_len_ptr > 0 ) {
        if (*running_head_ptr != NULL) {
            int dummy_len = 0;
            transition_process(running_head_ptr, &dummy_len, rdy_q_head_ptr, rdy_q_len_ptr, READY);
        }
        run_process(running_head_ptr, rdy_q_head_ptr, rdy_q_len_ptr, event_q_head_ptr, event_q_len_ptr, sim_time);
    }
    else if (strcmp(sched, "SJF") != EQUAL && strcmp(sched, "RR") != EQUAL) {
        fprintf(stderr, "Invalid scheduling method\n");
		exit(EXIT_FAILURE);
    }
}


void run_process(ListNode** running_head_ptr, ListNode** rdy_q_head_ptr, int* rdy_q_len_ptr, ListNode** event_q_head_ptr, int* event_q_len_ptr, unsigned long sim_time) {

    int dummy_len = 0;
    Process* running_proc = transition_process(rdy_q_head_ptr, rdy_q_len_ptr, running_head_ptr, &dummy_len, RUNNING);
    // convert remaining time to string for event printing
    char remaining_time[MAX_INFO_LEN];
    sprintf(remaining_time, "%lu", running_proc->rem_service_time);
    add_event(event_q_head_ptr, event_q_len_ptr, sim_time, running_proc->state, running_proc->name, remaining_time);
}

Process* transition_process(ListNode** source_head_ptr, int* source_len_ptr, ListNode** dest_head_ptr, int* dest_len_ptr, State state) {
    Process* next_proc =  malloc(sizeof(Process));
    if (next_proc == NULL) {
        fprintf(stderr, "Malloc failure\n");
        exit(EXIT_FAILURE);
    }
    *next_proc = *((Process *)(*source_head_ptr)->element);
    next_proc->state = state;
    *source_head_ptr = dequeue(*source_head_ptr);
    *source_len_ptr -= 1;
    *dest_head_ptr = enqueue(*dest_head_ptr, next_proc);
    *dest_len_ptr += 1;
    return next_proc;
}



// Compares two processes according to SJF specifications
int sjf_cmp(ListNode* node1, ListNode* node2) {

    Process process1 = *((Process*)(node1->element));
    Process process2 = *((Process*)(node2->element));

    if (process1.service_time < process2.service_time) {
        return LESS_THAN;
    }
    else if (process1.service_time > process2.service_time) {
        return GREATER_THAN;
    }
    else if (process1.arrival_time < process2.arrival_time) {
        return LESS_THAN;
    }
    else if (process1.arrival_time > process2.arrival_time) {
        return GREATER_THAN;
    }
    else if (strcmp(process1.name, process2.name) == LESS_THAN) {
        return LESS_THAN;
    } 
    else if (strcmp(process1.name, process2.name) == GREATER_THAN) {
        return GREATER_THAN;
    }
    else {
        fprintf(stderr, "SJF Tiebreaking Error - Processes don't have unique names\n");
		exit(EXIT_FAILURE);
    }
}