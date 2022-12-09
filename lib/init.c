#include "lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int init_global_vars(){
     // Init job_mutexes
    for (int i = 0; i < MAX_JOBS; i++)
        if (pthread_mutex_init(&job_mutexes[i], NULL))
        {
            perror(NULL);
            return errno;
        }

    // Init stats
    for (int i = 0; i < MAX_JOBS; i++)
    {
        copyjob_stats job_stats;
        job_stats.state = AVAILABLE;
        jobs_stats[i] = job_stats;
    }
    
    // Init semaphore
    if (sem_init(&semaphore, 0, MAX_THREADS))
    {
        perror(NULL);
        return errno;
    }

    return 0;
}