#include "task4.h"

/* Creates a real process by forking and execing process*/
void create_real_process(Process* proc, unsigned long sim_time) {
 
    int fd1[2];
    int fd2[2];
    char* args[] = {"-v", proc->name,  NULL};
    pid_t childpid;
    if (pipe(fd1) == -1) {
        printf("Pipe error\n");
        exit(EXIT_FAILURE);
    }
    if (pipe(fd2) == -1) {
        printf("Pipe error\n");
        exit(EXIT_FAILURE);
    }
    proc->proc_read = fd2[0];
    proc->proc_write = fd1[1];
    proc->mngr_read = fd1[0];
    proc->mngr_write = fd2[1];
    proc->run_1_quantum = false;

    int proc_read = proc->proc_read;
    int proc_write = proc->proc_write;
    int mngr_read = proc->mngr_read;
    int mngr_write = proc->mngr_write;

    if ((childpid = fork()) == -1) {
        perror("Fork error\n");
        exit(EXIT_FAILURE);
    }
    // Real process
    if (childpid == 0) {
        close(mngr_read); // Close the unwanted read side
        close(mngr_write); // Close the unwanted write side
        dup2(proc_read, STDIN_FILENO);
        dup2(proc_write, STDOUT_FILENO);
        execv("process", args);
    }
    // Process manager 
    else {
        proc->pid = childpid;
        close(proc_write); // Close the unwanted write side
        close(proc_read); // Close the unwanted read side
        send_32bit_sim_time(sim_time, mngr_write);
        verify_byte(sim_time, mngr_read);
    }
}

/* Suspends a real process by sending a stop signal */
void suspend_real_process(Process* proc, unsigned long sim_time) {
    int mngr_write = proc->mngr_write;
    proc->run_1_quantum = false;

    send_32bit_sim_time(sim_time, mngr_write);

    kill(proc->pid, SIGTSTP);
    int wstatus;
    pid_t w;
    do {
        w = waitpid(proc->pid, &wstatus, WUNTRACED);
        if (w == -1) {
            perror("waitpid error\n");
            exit(EXIT_FAILURE);
        }

        if (WIFSTOPPED(wstatus)) {
            return;
        }
    } while (!WIFSTOPPED(wstatus)); 
    
}

/* Continues a real process by sending a continue signal */
void continue_real_process(Process* proc, unsigned long sim_time) {
    int mngr_read = proc->mngr_read;
    int mngr_write = proc->mngr_write;
   
    send_32bit_sim_time(sim_time, mngr_write);

    kill(proc->pid, SIGCONT);
    verify_byte(sim_time, mngr_read);
}

/* Resumes a (presumably suspended) real process by sending a continue signal */
void resume_real_process(Process* proc, unsigned long sim_time) {
    continue_real_process(proc, sim_time);
}

/* Terminates a real process by sending a terminate signal */
void terminate_real_process(Process* proc, unsigned long sim_time) {
    int mngr_read = proc->mngr_read;
    int mngr_write = proc->mngr_write;
   
    char sha[SHA_LEN];
    send_32bit_sim_time(sim_time, mngr_write);

    kill(proc->pid, SIGTERM);
    int wstatus;
    pid_t w;
    do {
        w = waitpid(proc->pid, &wstatus, WUNTRACED);
        if (w == -1) {
            perror("waitpid error\n");
            exit(EXIT_FAILURE);
        }
        if (WIFEXITED(wstatus)) {
            read(mngr_read, sha, 64);
            sha[64] = '\0';
            strcpy(proc->sha, sha);
        }
    } while (!WIFEXITED(wstatus)); 
}

/* Sends the 32 bit simulation time in Big Endian Byte Ordering to the provided file 
   descriptor */
void send_32bit_sim_time(unsigned long sim_time, int mngr_write) {

    u_int8_t bytes[4];
    *(u_int32_t *) bytes = sim_time;

    convert_big_endian(sim_time, bytes);

    write(mngr_write, bytes, sizeof(bytes));
}

/* Verifies that the first byte provided in the file descriptor matches the least 
   significant byte of simulation time*/
void verify_byte(unsigned long sim_time, int mngr_read) {
    u_int8_t bytes[4];

    convert_big_endian(sim_time, bytes);

    char buf[4];
    read(mngr_read, buf, 1);
    if (*(u_int8_t *) buf != bytes[3]) {
        printf("Error - Least significant byte does not match\n");
        exit(EXIT_FAILURE);
    } 
}

/* Converts a 32 bit unsigned long to a byte array with Big Endian Ordering */
void convert_big_endian(unsigned long sim_time, u_int8_t bytes[4]) {

    unsigned int big_end_sim_time = htonl(sim_time);
    memcpy(bytes, &big_end_sim_time, 4);
}