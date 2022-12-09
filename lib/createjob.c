#include "lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

int increase_copied_size(copyjob_t job_id, int size){
    pthread_mutex_lock(&job_mutexes[job_id]);
    jobs_stats[job_id].copied_size = jobs_stats[job_id].copied_size + size;
    pthread_mutex_unlock(&job_mutexes[job_id]);
}

void* copy_process(void* argv){
    copyjob_t job_id = *(int*)argv;
    sem_wait(&semaphore);
    printf("Processing JOB %d ...\n", job_id);
    sleep(3-job_id%3);
    
    printf("Procesed JOB %d\n", job_id);
    sem_post(&semaphore);
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

    struct stat src_stats;
	if (lstat(src, &src_stats) == -1) {
		perror("lstat");
		exit(EXIT_FAILURE);
		return 1;
	}

    int dest_exists = 1;
	struct stat dest_stats;
	if (lstat(dst, &dest_stats) == -1) {
		dest_exists = 0;
	}

    int source_fd = open(src, O_RDONLY);
	if (source_fd < 0) {
        perror("Can not open source file!");
		return errno;
	};

    char overwrite[1];
	if (dest_exists){
		printf("In file '%s' you already have something, do you want to OVERWRITE! [y/n]: ", dst);
		scanf("%s", overwrite);

		if (strcmp(overwrite, "Y")|| strcmp(overwrite, "y")){
            if (unlink(dst)){
                perror("Can not delete destination file!\n");
                return errno;
            }
            printf("File '%s' has been succesful deleted!\n", dst);

        } else {
			printf("Canceled!\n");
			return 0;
		}
	}

	int dest_fd = open(dst, O_WRONLY|O_CREAT, S_IRUSR | S_IWUSR);
	if (dest_fd < 0) {
        perror("Can not open destination file!\n");
		return errno;
	};

    
    // Allocate a job_id
    for (copyjob_t i = 0; i<MAX_JOBS; i++)
    {
        pthread_mutex_lock(&job_mutexes[i]);
        if (jobs_stats[i].state == AVAILABLE){
            job_id = i;
            copyjob_stats job_stats;
            strcpy(job_stats.src, src);
            strcpy(job_stats.dst, dst);
            job_stats.state = WAITING;
            job_stats.total_size = src_stats.st_size;
            job_stats.copied_size = 0;
            jobs_stats[i] = job_stats;
        }
        pthread_mutex_unlock(&job_mutexes[i]);
        if (job_id != -1) break;
    }
    
    if (job_id == -1) printf("Job otherflow! Too may copy jobs!\n");
    else {
        pthread_t thr;
        int * argv_job_id = (int*)malloc(sizeof(int));
        *argv_job_id = job_id;
        if ( pthread_create(&thr, NULL, copy_process, argv_job_id)) {
            perror(NULL);
            return errno;
        }
    }

    return job_id;
};