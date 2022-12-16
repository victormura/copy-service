#include "lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

void* resume_process(void* argv){
    copyjob_t job = *(int*)argv;
    // Wait for Resources
    sem_wait(&semaphore);
    // Update State
    pthread_mutex_lock(&job_stats_mutexes[job]);
    jobs_stats[job].state = IN_PROGRESS;
    pthread_mutex_unlock(&job_stats_mutexes[job]);
    // Unlock Execution
    pthread_mutex_unlock(&job_mutexes[job]);
}

int copy_resume(copyjob_t job)
{
    // Update Job State
    pthread_mutex_lock(&job_stats_mutexes[job]);
    jobs_stats[job].state = WAITING;
    pthread_mutex_unlock(&job_stats_mutexes[job]);
    
    // Create a thread for waiting the semaphore
    pthread_t thr;
    int *argv_job_id = (int*)malloc(sizeof(int));
    *argv_job_id = job;
    if (pthread_create(&thr, NULL, resume_process, argv_job_id)){
        perror(NULL);
        return errno;
    }
    return 0;
};