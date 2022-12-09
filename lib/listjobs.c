#include "lib.h"
#include <stdio.h>
#include <stdlib.h>

int copy_listjobs(copyjob_stats **statslist, unsigned int *jobscount)
{
    // list: 
    // # JOB ID | Progress              | State
    // JOB 1    | ######............... | IN PROGRESS
    // JOB 2    | ###########.......... | IN PROGRESS
    // JOB 3    | #######.............. | IN PROGRESS
    // JOB 4    | ..................... | WAITING
    // JOB 5    | ..................... | WAITING
    // JOB 6    | #########............ | PAUSED
    printf("List of jobs #%d!\n", *jobscount);
    return 0;
};