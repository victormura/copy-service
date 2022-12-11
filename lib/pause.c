#include "lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

int job_exists(copyjob_t job){
    if (job >= MAX_JOBS || job < 0){
        printf("Job %d doesn't exist!\n", job);
        return -1;
    }
    return 0;
}

int copy_pause(copyjob_t job)
{
    if(job_exists(job)) return -1;
    if (jobs_stats[job].state == AVAILABLE || jobs_stats[job].state == CANCELED){
        printf("Inactive process!\n");
        return -1;
    }
    if (jobs_stats[job].state == PAUSED){
        printf("Job %d is already paused!\n", job);
        return -1;
    }
    if (jobs_stats[job].state == IN_PROGRESS || jobs_stats[job].state == WAITING){
        pthread_mutex_lock(&job_mutexes[job]);
        pthread_mutex_lock(&job_stats_mutexes[job]);
        jobs_stats[job].state = PAUSED;
        pthread_mutex_unlock(&job_stats_mutexes[job]);
        sem_post(&semaphore);
        printf("Job %d paused!\n", job);
        return 0;
    }
};