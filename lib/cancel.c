#include "lib.h"
#include <stdio.h>
#include <stdlib.h>

int copy_cancel(copyjob_t job)
{
    /*
    cancel(job_id)
        mtx = job_mutexex[job_id];
        mutex_lock(&mtx);
        job_states[job_id] = CANCEL;
        remove dst;
        mutex_unlock(&mtx)
    */
    printf("Hello cancel!");
    return 0;
};
