#include "lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int copy_progress(copyjob_t job_id){
    // Get Job stats
    pthread_mutex_lock(&job_stats_mutexes[job_id]);
    int total_size = jobs_stats[job_id].total_size;
    int copied_size = jobs_stats[job_id].copied_size;
    int job_state = jobs_stats[job_id].state;
    pthread_mutex_unlock(&job_stats_mutexes[job_id]);

    // Calculate Progress Percentage
    int PERCENTAGE_MULTIPLE = 40;
    int percentage = (int)((copied_size / (float)total_size) * PERCENTAGE_MULTIPLE);

    // Build State Verbose Value
    char string_job_state[C_STATE_LENGTH];
    if(job_state == IN_PROGRESS)
        strcpy(string_job_state, "IN_PROGRESS");
    else if(job_state == WAITING)
        strcpy(string_job_state, "WAITING");
    else if(job_state == PAUSED)
        strcpy(string_job_state, "PAUSED");
    
    // Print Job Stats Row
    if(job_state == IN_PROGRESS || job_state == WAITING || job_state == PAUSED){
        char * progress = (char*)malloc(sizeof(char)*(percentage));
        for (int i = 0; i < percentage; i++) progress[i] = '#';

        char job_value[C_JOB_ID_LENGTH];
        sprintf(job_value, "JOB %d", job_id);
        printf(
            "%s | 0 [%s] 100 | %s \n",
            generate_cell(job_value, C_JOB_ID_LENGTH),
            generate_cell(progress, PERCENTAGE_MULTIPLE),
            generate_cell(string_job_state, C_STATE_LENGTH)
        );
    }
    else printf("Job does not exist!\n");
    return 0;
};