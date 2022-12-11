#include "lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int copy_progress(copyjob_t job_id){
       
    pthread_mutex_lock(&job_stats_mutexes[job_id]);
    int total_size = jobs_stats[job_id].total_size;
    int copied_size = jobs_stats[job_id].copied_size;
    int job_state = jobs_stats[job_id].state;
    pthread_mutex_unlock(&job_stats_mutexes[job_id]);

    int percentage = (int)((copied_size / (float)total_size) * 100);

    char string_job_state[20];
    if(job_state == IN_PROGRESS)
        strcpy(string_job_state, "IN_PROGRESS");
    else if(job_state == WAITING)
            strcpy(string_job_state, "WAITING");
        else if(job_state == PAUSED)
                strcpy(string_job_state, "PAUSED");
    
    if(job_state == IN_PROGRESS || job_state == WAITING || job_state == PAUSED){

        if(job_id < 10) printf("JOB %d  |  ", job_id);
        else printf("JOB %d |  ", job_id);
        printf("0 [");
        for(int i = 1; i <= percentage; i++)
            printf("|");
        for(int i = percentage+1; i <= 100; i++)
            printf(" ");
        printf("] 100  |  %s\n", string_job_state);
    }
    else printf("Job does not exist!\n");
    return 0;
};