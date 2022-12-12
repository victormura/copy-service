#include "test_config.h"
#include "../lib/lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <ctype.h>
#include <unistd.h>

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
        copyjob_t job_id = copy_createjob("tests/src/test.txt", dst);
        fflush(NULL);
    }

    copyjob_t test_job_1 = 5;
    copyjob_t test_job_2 = 7;

    copy_pause(test_job_1);
    if (jobs_stats[test_job_1].state == PAUSED) printf("Passed!\n");
    else printf("Failed!\n");
    copy_resume(test_job_1);

    if (jobs_stats[test_job_1].state == WAITING || jobs_stats[test_job_1].state == IN_PROGRESS) printf("Passed!\n");
    else printf("Failed!\n");

    copy_pause(test_job_2);
    if (jobs_stats[test_job_2].state == PAUSED) printf("Passed!\n");
    else printf("Failed!\n");

    copy_resume(test_job_2);
    if (jobs_stats[test_job_2].state == WAITING || jobs_stats[test_job_2].state == IN_PROGRESS) printf("Passed!\n");
    else printf("Failed!\n");

    for (int i = 0; i < MAX_THREADS; i++){
        sem_wait(&semaphore);
    }
    return 0;
}
