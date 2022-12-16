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

int safe_quit()
{
    int safe_quit = 1;
    for (int i = 0; i < MAX_JOBS; i++){
        if(jobs_stats[i].state != AVAILABLE){
            safe_quit = 0;
        }
    }
    if (safe_quit)
        return 0;
    
    printf("You still have some copy jobs in progress or paused! Cancel ALL jobs | Unpause and Wait ALL jobs | Abort action [c|w|a]");
    char response[3];
    if(!fgets(response, 3, stdin)) return -1;
    response[strcspn(response, "\n")] = 0;

    // Cancel all running jobs and quit
    if (strcmp(response, "c") == 0){
        for (int i = 0; i < MAX_THREADS; i++){
            pthread_mutex_lock(&job_stats_mutexes[i]);
            if (jobs_stats[i].state != AVAILABLE) 
                {
                    pthread_mutex_unlock(&job_stats_mutexes[i]);
                    copy_cancel(i);
                }
            pthread_mutex_unlock(&job_stats_mutexes[i]);
        }
    } 

    // Resume and wait for all jobs to finish and quit
    else if ((strcmp(response, "w") == 0)) {
        // Wait until all jobs will finish
        for (int i = 0; i < MAX_JOBS; i++){
            pthread_mutex_lock(&job_stats_mutexes[i]);
            if (jobs_stats[i].state == PAUSED) 
            {
                pthread_mutex_unlock(&job_stats_mutexes[i]);
                copy_resume(i);
            }
            pthread_mutex_unlock(&job_stats_mutexes[i]);
        }
        printf("Waiting");
        fflush(NULL);
        for (int i = 0; i < MAX_THREADS; i++){
            if (i % (MAX_THREADS/5) == 0) printf(".");
            fflush(NULL);
            sem_wait(&semaphore);
        }
        printf("\n");
    } 
    
    // Abort quit action
    else return -1;
    return 0;
}

char * generate_cell(const char * cell_value, int max_column_length){

    // Add spaces for stats table row
    int cell_value_len = strlen(cell_value); 
    char * column = (char *)malloc(sizeof(char) * (max_column_length+1));
    for (int i = 0; i<cell_value_len; i++) column[i] = cell_value[i];
    for (int i = cell_value_len; i<max_column_length; i++) column[i] = ' ';
    column[max_column_length] = '\0';
    return column;
}

int job_validate(copyjob_t job){
    if (job >= MAX_JOBS || job < 0){
        printf("Job %d doesn't exist!\n", job);
        return -1;
    }

    pthread_mutex_lock(&job_stats_mutexes[job]);
    if (jobs_stats[job].state == AVAILABLE){
        printf("Job %d doesn't exist!\n", job);
        pthread_mutex_unlock(&job_stats_mutexes[job]);
        return -1;
    }
    pthread_mutex_unlock(&job_stats_mutexes[job]);
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