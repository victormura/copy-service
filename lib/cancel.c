#include "lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int copy_cancel(copyjob_t job)
{
    pthread_mutex_lock(&job_stats_mutexes[job]);
    close(jobs_stats[job].dst_fd);
    close(jobs_stats[job].src_fd);
    printf("Hello cancel!");
        jobs_stats[job].state = AVAILABLE;
        if (unlink(jobs_stats[job].dst))
        {
            perror("Can not delete destination file!\n");
            return -1;
        }
    pthread_mutex_unlock(&job_stats_mutexes[job]);
    sem_post(&semaphore);
    return 0;
};
