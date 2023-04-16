#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "process_utils.h"


/* Creates a real process by forking and execing process*/
void create_real_process(Process* proc, unsigned long sim_time);

/* Suspends a real process by sending a stop signal */
void suspend_real_process(Process* proc, unsigned long sim_time);

/* Continues a real process by sending a continue signal */
void continue_real_process(Process* proc, unsigned long sim_time);

/* Resumes a (presumably suspended) real process by sending a continue signal */
void resume_real_process(Process* proc, unsigned long sim_time);

/* Terminates a real process by sending a terminate signal */
void terminate_real_process(Process* proc, unsigned long sim_time);

/* Sends the 32 bit simulation time in Big Endian Byte Ordering to the provided file 
   descriptor */
void send_32bit_sim_time(unsigned long sim_time, int mngr_write);

/* Verifies that the first byte provided in the file descriptor matches the least significant 
   byte of simulation time*/
void verify_byte(unsigned long sim_time, int mngr_read);

/* Converts a 32 bit unsigned long to a byte array with Big Endian Ordering */
void convert_big_endian(unsigned long sim_time, u_int8_t bytes[4]);

