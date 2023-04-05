//#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

// reads the value of each of the command line arguments
char** read_args(int argc, char** argv) {

    char* fvalue = NULL;
    char* svalue = NULL;
    char* mvalue = NULL;
    char* qvalue = NULL;
    int index;
    int c;

    opterr = 0;

    while ((c = getopt(argc, argv, "f:s:m:q:")) != -1)
	    switch (c)
	    {
	    case 'f':
		    fvalue = optarg;
		    break;
	    case 's':
		    svalue = optarg;
		    break;
	    case 'm':
		    mvalue = optarg;
		    break;
        case 'q':
		    qvalue = optarg;
		    break;
	    case '?':
		    if (optopt == 'f' || optopt == 's' || optopt == 'm' || optopt == 'q')
		        fprintf (stderr, "Option -%c requires an argument.\n", optopt);
		    else if (isprint(optopt))
		        fprintf (stderr, "Unknown option `-%c'.\n", optopt);
		    else
		        fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
		    return NULL;
	    default:
		    abort ();
	  }

    printf ("fvalue = %s, svalue = %s, mvalue = %s, qvalue = %s\n", fvalue, svalue, mvalue, qvalue);

    for (index = optind; index < argc; index++)
	    printf ("Non-option argument %s\n", argv[index]);

    char* argvalues[4] = {fvalue, svalue, mvalue, qvalue}; // maybe try to store the index of the relevant args in argv rather than pointers to the values
    return argvalues;
}

void read_process_file(char* filepath) {

}