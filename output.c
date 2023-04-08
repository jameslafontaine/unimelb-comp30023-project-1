#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "output.h"




void calculate_performance_stats(ListNode* fnsh_q_head, int fnsh_q_len, unsigned long sim_time) {
    unsigned long ave_turnaround = 0;
    unsigned long ave_overhead = 0;
    unsigned long max_overhead = 0;
}

void add_event(ListNode** event_q_head_ptr, int* event_q_len_ptr, unsigned long sim_time, State state, char proc_name[MAX_NAME_LEN], char info[MAX_INFO_LEN]) {
    Event* new_event = malloc(sizeof(Event));
    if (new_event == NULL) {
        fprintf(stderr, "Malloc error\n");
        exit(EXIT_FAILURE);
    }
    new_event->sim_time = sim_time;
    new_event->state = state;
    strcpy(new_event->proc_name, proc_name);
    strcpy(new_event->info, info);
    
    *event_q_head_ptr = enqueue(event_q_head_ptr, new_event);
    *event_q_len_ptr += 1;
}

void print_events(ListNode** event_q_head_ptr, int* event_q_len_ptr) {
    if (*event_q_len_ptr > 0) {
        Event next_event;
        static int (*cmp_func)(ListNode**, ListNode**);
        cmp_func = event_cmp;
        ins_sort_list(event_q_head_ptr, cmp_func);
        while (*event_q_len_ptr > 0) {
            next_event = *((Event *)(*event_q_head_ptr)->element);
            if (next_event.state == READY) {
                printf("%lu,READY,process_name=%s,assigned_at=%s\n", next_event.sim_time, next_event.proc_name, next_event.info);
            }
            else if (next_event.state == RUNNING) {
                printf("%lu,RUNNING,process_name=%s,remaining_time=%s\n", next_event.sim_time, next_event.proc_name, next_event.info);
            }
            else if (next_event.state == FINISHED) {
                printf("%lu,FINISHED,process_name=%s,proc_remaining=%s\n", next_event.sim_time, next_event.proc_name, next_event.info);
            }
            else if (next_event.state == FINISHED_PROCESS) {
                printf("%lu,FINISHED-PROCESS,process_name=%s,sha=%s\n", next_event.sim_time, next_event.proc_name, next_event.info);
            }
            *event_q_head_ptr = dequeue(*event_q_head_ptr);
            *event_q_len_ptr -= 1;
        }
    }
}

int event_cmp(ListNode** node1, ListNode** node2) {

    Event event1 = *((Event*)(*node1)->element);
    Event event2 = *((Event*)(*node2)->element);

    if (event1.state < event2.state) {
        return LESS_THAN;
    }
    else if (event1.state > event2.state) {
        return GREATER_THAN;
    }
    else {
        return EQUAL;
    }
}