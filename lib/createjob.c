#include "lib.h"
#include <stdio.h>
#include <stdlib.h>

// managing threads
// void* thread_allocation(void * argv){
//     /*
//     -- alocare threduri
//     global mutex_thread
//     threads_list = [-1, -1, -1, ..., -1] -> 10
//     job_threads[job_id] = -1
//     while (job_threads[job_id] == -1){
//         mutex_lock(mutex_thread);
//         for i in threads_list
//             if i == -1
//                 job_threads[job_id] = i
//                 break;
//         mutex_unlock(mutex_thread)
//     }
//     threads_list = [10, 15, -1, 10, ..., -1]


//     */
// }

void* copy_process(void* argv){
    /*
    sem_wait(&semaphore);
    job_id <- argv;
    src_fd <- stats;
    dst_fd <- stats;
    copied_size = 0;
    file_size = src_stats.size;
    mtx = job_mutexes[job_id];
    while (copied_size < file_size){
        mutex_lock(&mtx);
        if (job_states[job_id] == CANCEL) {sem_post(&semaphore); return -1;}
        src_batch = copy 4k from src;
        write 4k in dst
        change copy stats
        mutex_unlock(&mtx);
    }
    sem_post(&semaphore);
    return 0;
    */
}


copyjob_t copy_createjob(const char *src, const char *dst)
{
    printf("Copying from %s to %s!\n", src, dst);
    // job_stats
    copyjob_t job_id = -1;
    /*
    thread 
    thread_join(&thread)
    thread_wait(&thread) unde?
    job_id = -1
    for i in range(NR_JOB):
        if job_stats[job_id].state == AVIALABLE
            job_id = i
            job_stats clean up
            job_stats.dst = dst;
            job_stats.src = src;
            job_stats.total_size = src_stats.size;
            job_stats.copied_size = 0;
            job_stats.state = WAITING;
        
    
    if job_id == -1:
        printf("Job overflow!")
    */

    // thread copy_process init and don't wait
    return job_id;
};