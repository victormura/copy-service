#include "lib.h"
#include <stdio.h>
#include <stdlib.h>

int copy_pause(copyjob_t job)
{
    /*
    mtx = job_mutexex[job_id];
    mutex_lock(&mtx);
    job_states[job_id] = PAUSED
    semaphore_post(&semaphore);
    */
    printf("Pause JOB %d!\n", job);
    return 0;
};