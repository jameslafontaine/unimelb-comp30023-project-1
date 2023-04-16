#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "process_manager.h"
#include "output.h"

#define IMPLEMENTS_REAL_PROCESS
#ifdef IMPLEMENTS_REAL_PROCESS
#include "task4.h"
#endif

// #define PMDEBUG

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

    memory_list_head = initialise_memory(memory_list_head);

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
        if (simulation_time > 400) {
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

    #ifdef PMDEBUG
    print_list(memory_list_head);
    ListNode* current = memory_list_head;
    while (current) {
        print_memory_block((MemoryBlock *) current->element);
        current = current->next;
    }
    #endif

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

        #ifdef IMPLEMENTS_REAL_PROCESS
        running_proc->run_1_quantum = true;
        #endif

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
            char remaining_procs[MAX_INFO_LEN];
            sprintf(remaining_procs, "%d", *rdy_q_len_ptr + *in_q_len_ptr);
            add_event(event_q_head_ptr, event_q_len_ptr, sim_time, running_proc->state, running_proc->name, remaining_procs);

            #ifdef IMPLEMENTS_REAL_PROCESS
            // terminate process and retrieve the sha value
            terminate_real_process(running_proc, sim_time);
            add_event(event_q_head_ptr, event_q_len_ptr, sim_time, FINISHED_PROCESS, running_proc->name, running_proc->sha);
            #endif

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
        int initial_length = *in_q_len_ptr;
        while (num_processes_checked < initial_length) {
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

ListNode* initialise_memory(ListNode* mem_list_head) {
    MemoryBlock* new_block = (MemoryBlock *) malloc(sizeof(MemoryBlock));
    if (new_block == NULL) {
        fprintf(stderr, "Malloc failure\n");
        exit(EXIT_FAILURE);
    }

    new_block->type = HOLE;
    new_block->start_address = 0;
    new_block->length = MAX_MEM;
    
    return enqueue(mem_list_head, new_block);
}


int best_fit_alloc(Process* process, ListNode** mem_list) {
    unsigned short best_fit_size = MAX_MEM + 1;
    ListNode* best_fit_node = NULL;
    ListNode* current = *mem_list;
    MemoryBlock* current_block = NULL;
    MemoryBlock* best_fit_block = NULL;

    process->has_mem_alloc = false;

    #ifdef PMDEBUG
    printf("Attempting to find best fit...\n");
    #endif

    // iterate through memory list until reach end and track smallest hole that is greater
    // than or equal to memory requirement of process
    while (current) {
        current_block = (MemoryBlock *) (current->element);
        #ifdef PMDEBUG
        print_memory_block(current_block);
        printf("process->memory_req = %hu\n", process->memory_req);
        printf("best_fit_size = %hu\n", best_fit_size);
        #endif
        if (current_block->type == HOLE && current_block->length >= process->memory_req && current_block->length < best_fit_size) {
            best_fit_size = current_block->length;
            best_fit_node = current;
            best_fit_block = (MemoryBlock *) (best_fit_node->element); 
            process->mem_address = current_block->start_address;
            process->has_mem_alloc = true;
            // can stop if we find an exact fit as this won't be beaten by other blocks
            if (current_block->length == process->memory_req) {
                break;
            }
        }
        current = current->next;
    }

    #ifdef PMDEBUG
    printf("MODIFYING MEMORY LIST TO REPRESENT ALLOCATION\n");
    #endif
    if (process->has_mem_alloc == true) {
        // modify memory list to represent the allocation 
        // if the memory requirement is smaller than the size of the hole then we must reserve the required
        // amount of memory and leave the rest as a hole
        if (process->memory_req < best_fit_size) {
            MemoryBlock* new_hole = (MemoryBlock *) malloc(sizeof(MemoryBlock));
            if (new_hole == NULL) {
                fprintf(stderr, "Malloc failure\n");
                exit(EXIT_FAILURE);
            }
            new_hole->type = HOLE;
            new_hole->start_address = best_fit_block->start_address + process->memory_req;
            new_hole->length = best_fit_block->length - process->memory_req;
            best_fit_block->type = PROCESS;
            best_fit_block->length = process->memory_req;
            best_fit_node = insert_after_node(best_fit_node, new_hole);
            //best_fit_node = insert_at_tail(best_fit_node, new_hole);
            //if (best_fit_node->prev != NULL) {
            //    (best_fit_node->prev)->next = best_fit_node;
            //}
            //#ifdef PMDEBUG
            //print_memory_block(best_fit_block);
            //print_memory_block(new_hole);
            //print_list(*mem_list);
            //#endif
        }
        // if the fit is exact then we simply change the block type from hole to process
        else if (process->memory_req == best_fit_size) {
            best_fit_block->type = PROCESS;
        }
        #ifdef PMDEBUG
        print_list(*mem_list);
        ListNode* current = *mem_list;
        while (current) {
            print_memory_block((MemoryBlock *) current->element);
            current = current->next;
        }
        #endif 
        return SUCCESS;
    }
    else {
        return FAIL;
    }
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

// remove process from memory list and join holes as needed
void best_fit_dealloc(Process* process, ListNode** mem_list) {
    
    ListNode* allocated_node = NULL;
    ListNode* current = *mem_list;
    MemoryBlock* current_block = NULL;
    MemoryBlock* allocated_block = NULL;
    ListNode* prev_node = NULL;
    MemoryBlock* prev_block = NULL;
    ListNode* next_node = NULL;
    MemoryBlock* next_block = NULL;

    process->has_mem_alloc = false;

    // iterate through memory list until reach allocated block for this process and remove it
    // while filling in holes
    while (current) {
        current_block = (MemoryBlock *) (current->element);
        if (current_block->start_address == process->mem_address) {
            allocated_node = current;
            allocated_block = current_block;
            if (allocated_node->prev != NULL) {
                prev_node = allocated_node->prev;
                prev_block = (MemoryBlock *) prev_node->element;
            }
            if (allocated_node->next != NULL) {
                next_node = allocated_node->next;
                next_block = (MemoryBlock *) next_node->element;
            }
            
            // if blocks to left and right are processes then we simply change the type of this block
            // to a hole, otherwise we must also join holes
            allocated_block->type = HOLE;
            if (prev_node != NULL && prev_block->type == HOLE) {
                allocated_block->length += prev_block->length;
                allocated_block->start_address = prev_block->start_address;
                allocated_node->prev = prev_node->prev;
                if (allocated_node->prev != NULL) {
                    (prev_node->prev)->next = allocated_node;
                }
                // set memory list head to allocated node if it is the new head
                else { 
                (*mem_list = allocated_node);
                }
                free_node(&prev_node);
            }
            if (next_node != NULL && next_block->type == HOLE) {
                allocated_block->length += next_block->length;
                allocated_node->next = next_node->next;
                if (allocated_node->next != NULL) {
                    (allocated_node->next)->prev = allocated_node;
                }
                free_node(&next_node);
            }
            #ifdef PMDEBUG
            print_list(*mem_list);
            ListNode* current = *mem_list;
            while (current) {
                print_memory_block((MemoryBlock *) current->element);
                current = current->next;
            }
            #endif
            break;
        }
        current = current->next;    
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

        #ifdef IMPLEMENTS_REAL_PROCESS
        // create a real process using process.c as this must be the process' first time running
        create_real_process((Process *) (*running_head_ptr)->element, sim_time);
        #endif
    }
    // if the scheduling method is round robin then simply let the next process in the ready queue run if
    // there is one by preempting the currently running process, otherwise give more CPU time to the currently running process
    else if (strcmp(sched, "RR") == EQUAL && *rdy_q_len_ptr > 0 ) {
        if (*running_head_ptr != NULL) {
            
            #ifdef IMPLEMENTS_REAL_PROCESS
            // suspend the currently running process
            suspend_real_process((Process *) (*running_head_ptr)->element, sim_time);
            #endif

            int dummy_len = 0;
            transition_process(running_head_ptr, &dummy_len, rdy_q_head_ptr, rdy_q_len_ptr, READY);
        }
        run_process(running_head_ptr, rdy_q_head_ptr, rdy_q_len_ptr, event_q_head_ptr, event_q_len_ptr, sim_time);

        #ifdef IMPLEMENTS_REAL_PROCESS
        // create a real process if this is the process' first time running
        Process* running_proc = (Process *) (*running_head_ptr)->element;
        if (running_proc->service_time == running_proc->rem_service_time) {
            create_real_process(running_proc, sim_time);
        }
        // otherwise the process is being resumed
        else {
            resume_real_process(running_proc, sim_time);
        }
        #endif
        
    }
    else if (strcmp(sched, "SJF") != EQUAL && strcmp(sched, "RR") != EQUAL) {
        fprintf(stderr, "Invalid scheduling method\n");
		exit(EXIT_FAILURE);
    }

    #ifdef IMPLEMENTS_REAL_PROCESS
    // if we are letting the currently running process have more CPU time then it is being continued, (except
    // for the cycle on which it was created or resumed running)
    else if (*running_head_ptr != NULL) {
        Process* running_proc = (Process *) ((*running_head_ptr)->element);
        if (strcmp(sched, "SJF") == EQUAL && running_proc->run_1_quantum) {
            continue_real_process((Process *) (*running_head_ptr)->element, sim_time);
        }
        else if ((strcmp(sched, "RR") == EQUAL && *rdy_q_len_ptr == 0 && running_proc->run_1_quantum)) {
            continue_real_process((Process *) (*running_head_ptr)->element, sim_time);
        }
    }    
    #endif

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
    *dest_head_ptr = enqueue(*dest_head_ptr, next_proc); // could pass pointer into function from source queue rather than malloc a new process pointer,
    *dest_len_ptr += 1;                                  // but would have to remove free(node->element in d_linked_list.c)
    return next_proc;
}



// Compares two processes according to SJF specifications
int sjf_cmp(ListNode* node1, ListNode* node2) {

    Process process1 = *((Process*)(node1->element));
    Process process2 = *((Process*)(node2->element));

    #ifdef PMDEBUG
    printf("Process 1 service = %lu\n", process1.service_time);
    printf("Process 2 service = %lu\n", process2.service_time);
    printf("Process 1 arrival = %lu\n", process1.arrival_time);
    printf("Process 2 arrival = %lu\n", process2.arrival_time);
    printf("Process 1 name = %s\n", process1.name);
    printf("Process 2 name = %s\n", process2.name);
    #endif

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
    else if (strcmp(process1.name, process2.name) < EQUAL) {
        return LESS_THAN;
    } 
    else if (strcmp(process1.name, process2.name) > EQUAL) {
        return GREATER_THAN;
    }
    else {
        fprintf(stderr, "SJF Tiebreaking Error - Processes don't have unique names\n");
		exit(EXIT_FAILURE);
    }
}

void print_memory_block(MemoryBlock* mem_block) {
    printf("###################################\n");
    printf("mem_block->type = %d\n", mem_block->type);
    printf("mem_block->start_address = %hu\n", mem_block->start_address);
    printf("mem_block->length = %hu\n", mem_block->length);
    printf("###################################\n");
}

