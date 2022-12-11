#include "lib.h"
#include <stdio.h>
#include <stdlib.h>

int copy_listjobs()
{
    // list: 
    // # JOB ID | Progress              | State
    // JOB 1    | ######............... | IN PROGRESS
    // JOB 2    | ###########.......... | IN PROGRESS
    // JOB 3    | #######.............. | IN PROGRESS
    // JOB 4    | ..................... | WAITING
    // JOB 5    | ..................... | WAITING
    // JOB 6    | #########............ | PAUSED
    printf("JOB ID |                                                 PROGRESS                                                       |  State\n");
    for(copyjob_t job_id = 0; job_id < MAX_JOBS; job_id++)
        {
            pthread_mutex_lock(&job_stats_mutexes[job_id]);
            if(jobs_stats[job_id].state == WAITING || jobs_stats[job_id].state == IN_PROGRESS || jobs_stats[job_id].state == PAUSED)
                {
                    pthread_mutex_unlock(&job_stats_mutexes[job_id]);
                    copy_progress(job_id);
                }
            pthread_mutex_unlock(&job_stats_mutexes[job_id]);   
        }
    return 0;
};