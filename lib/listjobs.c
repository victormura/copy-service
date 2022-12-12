#include "lib.h"
#include <stdio.h>
#include <stdlib.h>

int copy_listjobs()
{
    // Print Header
    printf(
        "%s | %s | %s \n",
        generate_cell("JOB ID", C_JOB_ID_LENGTH),
        generate_cell("PROGRESS", C_PROGRESS_LENGTH),
        generate_cell("STATE", C_STATE_LENGTH)
    );

    // Print JOBS Progress
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