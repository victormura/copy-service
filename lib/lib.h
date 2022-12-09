#ifndef LIB_H_ /* Include guard */
#define LIB_H_

#include <pthread.h>
#include <semaphore.h>
#include "../config.h"

#define AVAILABLE 0
#define IN_PROGRESS 1
#define WAITING 2
#define PAUSED 3

// Types
typedef int copyjob_t;

typedef struct
{
    char dst[256];
    int dst_fd;
    char src[256];
    int src_fd;
    int state; // INITIAL STATE AVAILABLE
    int total_size;
    int copied_size;
} copyjob_stats;

// Global vars
extern pthread_mutex_t job_mutexes[MAX_JOBS];
extern pthread_mutex_t job_stats_mutexes[MAX_JOBS];
extern sem_t semaphore;
extern copyjob_stats jobs_stats[MAX_JOBS];

int init_global_vars();

// Library interface
copyjob_t copy_createjob(const char *src, const char *dst);

int copy_cancel(copyjob_t job);

int copy_pause(copyjob_t job);

int copy_resume(copyjob_t job);

int copy_stats(copyjob_t job, copyjob_stats *stats);

int copy_progress(copyjob_t job);

int copy_listjobs(copyjob_stats **statslist, unsigned int *jobscount);

void copy_freestats(copyjob_stats *stats);

#endif // LIB_H_