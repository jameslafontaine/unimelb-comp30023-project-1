#include "process_utils.h"

#define FPATH 0
#define SCHEDULER 1
#define MEM_MNG 2
#define QUANTUM 3
#define NARGS 4


/* Reads in the specified argument values and returns pointers to those arguments
*/
char** read_args(int argc, char** argv);


/* Reads the process file located via the provided absolute or relative filepath and returns
   an array of those processes in the form of struct process defined in process_utils.h 
*/
Process* read_process_file(char* filepath);