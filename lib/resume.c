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
    sem_wait(&semaphore);
    pthread_mutex_lock(&job_stats_mutexes[job]);
    jobs_stats[job].state = IN_PROGRESS;
    pthread_mutex_unlock(&job_stats_mutexes[job]);
}

int copy_resume(copyjob_t job)
{
    if(job_exists(job)) return -1;
    pthread_mutex_lock(&job_stats_mutexes[job]);
    jobs_stats[job].state = WAITING;
    pthread_mutex_unlock(&job_stats_mutexes[job]);
    pthread_t thr;
    int *argv_job_id = (int*)malloc(sizeof(int));
    *argv_job_id = job;
    if (pthread_create(&thr, NULL, resume_process, argv_job_id)){
        perror(NULL);
        return errno;
    }
    pthread_mutex_unlock(&job_mutexes[job]);
    printf("Job %d resumed!\n", job);
    return 0;
};