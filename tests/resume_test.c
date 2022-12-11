#include "test_config.h"
#include "../lib/lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <ctype.h>

pthread_mutex_t job_mutexes[MAX_JOBS];
pthread_mutex_t job_stats_mutexes[MAX_JOBS];
sem_t semaphore;
copyjob_stats jobs_stats[MAX_JOBS];

int main(){
    if (init_global_vars()){
        return -1;
    }
    int nr_of_jobs = 50;
    for (int i = 0; i < nr_of_jobs; i++){
        char dst[256];
        sprintf(dst, "tests/dst/test%d.txt", i);
        copyjob_t job_id = copy_createjob("tests/src/stest.txt", dst);
        fflush(NULL);
    }
    copy_pause(5);
    copy_resume(5);
    copy_pause(7);
    copy_resume(7);
    for (int i = 0; i < MAX_THREADS; i++){
        sem_wait(&semaphore);
    }
    return 0;
}
