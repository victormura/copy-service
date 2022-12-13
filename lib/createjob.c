#include "lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

int increase_copied_size(copyjob_t job_id, int size){
    pthread_mutex_lock(&job_stats_mutexes[job_id]);
    pthread_mutex_unlock(&job_stats_mutexes[job_id]);
}

void* copy_process(void* argv){
    copyjob_t job_id = *(int*)argv;
    // Wait For Resources
    sem_wait(&semaphore);

    // Get JOB Stats
    pthread_mutex_lock(&job_stats_mutexes[job_id]);
    jobs_stats[job_id].state = IN_PROGRESS;
    int source_fd = jobs_stats[job_id].src_fd;
    int dest_fd = jobs_stats[job_id].dst_fd;
    int copied_size = jobs_stats[job_id].copied_size;
    int total_size = jobs_stats[job_id].total_size;
    pthread_mutex_unlock(&job_stats_mutexes[job_id]);

    // Start Copy Process
    while (copied_size < total_size){
        pthread_mutex_lock(&job_mutexes[job_id]);

        // Check if JOB is not CANCELED
        pthread_mutex_lock(&job_stats_mutexes[job_id]);
        if (jobs_stats[job_id].state == CANCELED){

            pthread_mutex_unlock(&job_stats_mutexes[job_id]);
            pthread_mutex_unlock(&job_mutexes[job_id]);

            break;
        }
        pthread_mutex_unlock(&job_stats_mutexes[job_id]);

        // Read
        char * buf = (char*)malloc(BATCH_SIZE);
		int read_size = read(source_fd, buf, BATCH_SIZE);
		if (read_size < 0) {
			perror ("Read error occured!");
            copy_cancel(job_id);
			return NULL;
		};

        // Write
        ssize_t write_size = write(dest_fd, buf, read_size);
        if (write_size < 0) {
            perror ("Read error occured!");
            copy_cancel(job_id);
            return NULL;
        };

        // Update Copied Size
        pthread_mutex_lock(&job_stats_mutexes[job_id]);
        copied_size = copied_size + write_size;
        jobs_stats[job_id].copied_size = copied_size;
        pthread_mutex_unlock(&job_stats_mutexes[job_id]);

		free(buf);
        pthread_mutex_unlock(&job_mutexes[job_id]);
    }

    // Deallocate JOB by setting AVAILABLE state
    pthread_mutex_lock(&job_stats_mutexes[job_id]);
    jobs_stats[job_id].state = AVAILABLE;
    pthread_mutex_unlock(&job_stats_mutexes[job_id]);

    // Free Resource
    sem_post(&semaphore);
}


copyjob_t copy_createjob(char *src, char *dst)
{
    copyjob_t job_id = -1;

    // Get Source File Stats
    struct stat src_stats;
	if (lstat(src, &src_stats) == -1) {
		perror("lstat");
		return -1;
	}

    // Check Destination File Exists
    int dest_exists = 1;
	struct stat dest_stats;
	if (lstat(dst, &dest_stats) == -1) {
		dest_exists = 0;
	}

    int source_fd = open(src, O_RDONLY);
	if (source_fd < 0) {
        perror("Can not open source file!");
		return -1;
	};

    // Ask user if OVERWRITE or not the file
    char overwrite[3];
	if (dest_exists){
		printf("In file '%s' you already have something, do you want to OVERWRITE! [y/n]: ", dst);
        if(!fgets(overwrite, 3, stdin)) return -1;
        overwrite[strcspn(overwrite, "\n")] = 0;
		if (strcmp(overwrite, "Y") == 0 || strcmp(overwrite, "y") == 0){
            // Delete FILE if OVERWRITE - YES
            if (unlink(dst)){
                perror("Can not delete destination file!\n");
                return -1;
            }
            printf("File '%s' has been succesful deleted!\n", dst);

        } else {
            // Cancel ACTION if OVERWRITE - NO
			printf("Canceled!\n");
			return -1;
		}

	}
 
    // Create and Open Destination File
	int dest_fd = open(dst, O_WRONLY|O_CREAT, S_IRUSR | S_IWUSR);
	if (dest_fd < 0) {
        perror("Can not open destination file!\n");
		return -1;
	};

    // Allocate a JOB ID
    for (copyjob_t i = 0; i<MAX_JOBS; i++)
    {
        if (jobs_stats[i].state == AVAILABLE){
            pthread_mutex_lock(&job_stats_mutexes[i]);
            job_id = i;
            copyjob_stats job_stats;
            job_stats.src = src;
            job_stats.src_fd = source_fd;
            job_stats.dst = dst;
            job_stats.dst_fd = dest_fd;
            job_stats.state = WAITING;
            job_stats.total_size = src_stats.st_size;
            job_stats.copied_size = 0;
            jobs_stats[i] = job_stats;
            pthread_mutex_unlock(&job_stats_mutexes[i]);
            break;
        }
    }

    // Create a copy thread
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