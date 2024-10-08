#include "process_utils.h"
#include "d_linked_list.h"

#define IMPLEMENTS_REAL_PROCESS

#define MAX_MEM 2048
#define SUCCESS 1
#define FAIL 0

typedef enum { HOLE = 0, PROCESS = 1} BlockType;
typedef struct memory_block MemoryBlock;

struct memory_block {
    BlockType type;
    unsigned short start_address;
    unsigned short length;
};




/* Run the simulation of the process manager */
void run_simulation(char* sched, char* mem_mng, char* Q, ProcessList* process_list);

/* Determines whether the currently running process (if any) has completed.
    If so it should be terminated and its memory deallocated before
    subsequent scheduling tasks are performed. */
void check_running_process(ListNode** running_head_ptr, int* in_q_len_ptr, 
                           ListNode** rdy_q_head_ptr, int* rdy_q_len_ptr, 
                           ListNode** fnsh_q_head_ptr, int* fnsh_q_len_ptr,
                           unsigned long sim_time, short quantum, char* sched, 
                           ListNode** mem_list_ptr, char* mem_mng, 
                           ListNode** event_q_head_ptr, int* event_q_len_ptr);

/* Identifies all processes that have been submitted to the system since
    the last cycle occurred and adds them to the input queue in the order 
    they appear in the process file. (Arrival time <= simulation time) */
void add_submitted_processes(ListNode** in_q_head_ptr, int* in_q_len_ptr, 
                             ProcessList** process_list_ptr, unsigned long sim_time, 
                             ListNode** event_q_head_ptr, int* event_q_len_ptr);

/* Initialises the memory linked list to have a 2048MB hole*/
ListNode* initialise_memory(ListNode* mem_list_head);

/* Moves processes from the input queue to the ready queue upon successful memory allocation */
void allocate_memory(ListNode** in_q_head_ptr, int* in_q_len_ptr, ListNode** rdy_q_head_ptr, 
                     int* rdy_q_len_ptr, ListNode** mem_list_ptr, char* mem_mng, 
                     unsigned long sim_time, ListNode** event_q_head_ptr, int* event_q_len_ptr);

/* Tries to allocate memory for the given process using best fit, returns success or failure int */
int best_fit_alloc(Process* process, ListNode** mem_list);

/* Deallocates memory for a finished process */ 
void deallocate_memory(ListNode** running_head_ptr, ListNode** mem_list_ptr, char* mem_mng);

/* Removes a process from the memory list and joins holes as needed */
void best_fit_dealloc(Process* process, ListNode** mem_list);

/* Determines the process (if any) which runs in this cycle */
void schedule_process(ListNode** running_head_ptr, ListNode** rdy_q_head_ptr, 
                      int* rdy_q_len_ptr, char* sched, unsigned long sim_time, 
                      ListNode** event_q_head_ptr, int* event_q_len_ptr);

/* Performs the necessary steps to begin running the next process in the ready queue */
void run_process(ListNode** running_head_ptr, ListNode** rdy_q_head_ptr, int* rdy_q_len_ptr, 
                 ListNode** event_q_head_ptr, int* event_q_len_ptr, unsigned long sim_time);

/* Moves a process from one queue/state to another */
Process* transition_process(ListNode** source_head_ptr, int* source_len_ptr, 
                            ListNode** dest_head_ptr, int* dest_len_ptr, State state);

/* Compares two processes according to SJF specifications */
int sjf_cmp(ListNode* node1, ListNode* node2);

/* Prints the contents of the supplied memory block from the linked list */
/*
void print_memory_block(MemoryBlock* mem_block);
*/