#include "lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int number_of_digits(int number){
    int count = 0;
    do {
        number /= 10;
        ++count;
    } while (number != 0);
    return count;
}

char * generate_cell(const char * cell_value, int max_column_length){
    int cell_value_len = strlen(cell_value);
    char * column = (char *)malloc(sizeof(char) * (max_column_length+1));
    for (int i = 0; i<cell_value_len; i++) column[i] = cell_value[i];
    for (int i = cell_value_len; i<max_column_length; i++) column[i] = ' ';
    return column;
}

int job_exists(copyjob_t job){
    if (job >= MAX_JOBS || job < 0){
        printf("Job %d doesn't exist!\n", job);
        return -1;
    }
    return 0;
}

int C_JOB_ID_LENGTH;
int C_PROGRESS_LENGTH;
int C_STATE_LENGTH;

int init_global_vars(){
    C_JOB_ID_LENGTH = strlen("JOB ") + number_of_digits(MAX_JOBS);
    C_PROGRESS_LENGTH = 48;
    C_STATE_LENGTH = 20;

    // Init job_mutexes
    for (int i = 0; i < MAX_JOBS; i++){
        if (pthread_mutex_init(&job_mutexes[i], NULL))
        {
            perror(NULL);
            return errno;
        }
        if (pthread_mutex_init(&job_stats_mutexes[i], NULL))
        {
            perror(NULL);
            return errno;
        }
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