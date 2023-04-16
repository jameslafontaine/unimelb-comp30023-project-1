#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "process_utils.h"

void create_real_process(Process* proc, unsigned long sim_time);

void suspend_real_process(Process* proc, unsigned long sim_time);

void continue_real_process(Process* proc, unsigned long sim_time);

void resume_real_process(Process* proc, unsigned long sim_time);

void terminate_real_process(Process* proc, unsigned long sim_time);

void send_32bit_sim_time(unsigned long sim_time, int mngr_write);

void verify_byte(unsigned long sim_time, int mngr_read);

void convert_big_endian(unsigned long sim_time, u_int8_t bytes[4]);

