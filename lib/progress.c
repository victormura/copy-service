#include "lib.h"
#include <stdio.h>
#include <stdlib.h>

int copy_progress(copyjob_t job){
    /*
    job_stats[job_id].total_size / copied_size -> current progress
    */
    printf("Progress JOB %d\n", job);
    return 0;
};