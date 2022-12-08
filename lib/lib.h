#ifndef LIB_H_ /* Include guard */
#define LIB_H_

#include <pthread.h>
#include <semaphore.h>
#include "../config.h"

// Types
typedef int copyjob_t;

struct copyjob_stats
{
    int foo;
};

// Global vars
extern pthread_mutex_t job_mutexes[MAX_JOBS];
extern sem_t semaphore;

// Library interface
copyjob_t copy_createjob(const char *src, const char *dst);

int copy_cancel(copyjob_t job);

int copy_pause(copyjob_t job);

int copy_resume(copyjob_t job);

int copy_stats(copyjob_t job, struct copyjob_stats *stats);

int copy_progress(copyjob_t job);

int copy_listjobs(struct copyjob_stats **statslist, unsigned int *jobscount);

void copy_freestats(struct copyjob_stats *stats);

#endif // LIB_H_