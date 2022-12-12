#include "lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int copy_cancel(copyjob_t job)
{
    // Get Job Stats
    pthread_mutex_lock(&job_stats_mutexes[job]);
    int job_state = jobs_stats[job].state;
    int dst_fd = jobs_stats[job].dst_fd;
    int src_fd = jobs_stats[job].src_fd;
    char * dst = jobs_stats[job].dst;
    pthread_mutex_unlock(&job_stats_mutexes[job]);

    // Lock Execution
    if (job_state != PAUSED) pthread_mutex_lock(&job_mutexes[job]);

    // Close Files
    close(dst_fd);
    close(src_fd);

    // Update JOB state
    pthread_mutex_lock(&job_stats_mutexes[job]);
    jobs_stats[job].state = CANCELED;
    pthread_mutex_unlock(&job_stats_mutexes[job]);

    // Delete Destination file
    if (unlink(dst))
    {
        perror("Can not delete destination file!\n");
        return -1;
    }

    // Unlock Execution
    pthread_mutex_unlock(&job_mutexes[job]);
    return 0;
};
