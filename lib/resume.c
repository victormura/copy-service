#include "lib.h"
#include <stdio.h>
#include <stdlib.h>

int copy_resume(copyjob_t job)
{
    /*
    mtx = job_mutexex[job_id];
    job_states[job_id] = WAITING
    semaphore_wait(&semaphore);
    mutex_unlock(&mtx);
    */
    printf("Resume JOB %d!\n", job);
    return 0;
};