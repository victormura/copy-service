// Damon file
#include "config.h"
#include "lib/lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <ctype.h>

// Define commands
#define INVALID_COMMAND -1
#define QUIT 0
#define CREATE 1
#define CANCEL 2
#define PAUSE 3
#define RESUME 4
#define STATS 5
#define LIST 6

// Global vars
pthread_mutex_t job_mutexes[MAX_JOBS];
pthread_mutex_t job_stats_mutexes[MAX_JOBS];
sem_t semaphore;
copyjob_stats jobs_stats[MAX_JOBS];

// Input command parser
int parse_command(char *command)
{
    for(int i = 0; command[i]; i++)
        command[i] = tolower(command[i]);
    if (strcmp(command, "cp") == 0 || strcmp(command, "copy") == 0) return CREATE;
    else if (strcmp(command, "ls") == 0 || strcmp(command, "list") == 0) return LIST;
    else if (strcmp(command, "quit") == 0 || strcmp(command, "q") == 0) return QUIT;
    else if (strcmp(command, "cancel") == 0) return CANCEL;
    else if (strcmp(command, "pause") == 0) return PAUSE;
    else if (strcmp(command, "resume") == 0) return RESUME;
    else if (strcmp(command, "stats") == 0) return STATS;
    return INVALID_COMMAND;
}

int call_command(int code)
{
    copyjob_t job_id;
    switch (code){
        case CREATE: ;
            // Create new copy JOB
            char *src, *dst;
            src = (char *)malloc(sizeof(char)*256);
            dst = (char *)malloc(sizeof(char)*256);
            scanf("%s", src);
            scanf("%s", dst);

            job_id = copy_createjob(src, dst);
            if (job_id == -1) {
                printf("Couldn't create a new job!\n");
                return 0;
            }
            printf("Copy job with ID=%d has started!\n", job_id);
            return 0;
        case LIST:
            // List all JOBS with their state (IN PROGRESS, WAITING, PAUSED)
            copy_listjobs();
            return 0;
        
        case PAUSE:
            // Pause JOB execution
            scanf("%d", &job_id);
            copy_pause(job_id);
            printf("Job %d paused!\n", job_id);
            return 0;
        case RESUME:
            // Resume JOB execution
            scanf("%d", &job_id);
            copy_resume(job_id);
            printf("Job %d resumed!\n", job_id);
            return 0;
        case STATS:
            // Optain JOB stats and progress
            scanf("%d", &job_id);
            copy_progress(job_id);
            return 0;
        case CANCEL:
            scanf("%d", &job_id);
            copy_cancel(job_id);
            printf("Cancel JOB %d\n", job_id);
            return 0;
        case QUIT: ;
            int safe_quit = 1;
            for (int i = 0; i < MAX_JOBS; i++){
                if(jobs_stats[i].state != AVAILABLE){
                    safe_quit = 0;
                }
            }
            if (!safe_quit){
                printf("You still have some copy jobs in progress or paused! Cancel ALL jobs | Unpause and Wait ALL jobs | Abort action [c|w|a]");
                char response[1];
                scanf("%s", response);
                if (strcmp(response, "c") == 0){
                    for (int i = 0; i < MAX_THREADS; i++){
                        if (jobs_stats[i].state != AVAILABLE) copy_cancel(i);
                    }
                } 
                else if ((strcmp(response, "w") == 0)) {
                    // Wait until all jobs will finish
                    for (int i = 0; i < MAX_THREADS; i++){
                        sem_wait(&semaphore);
                    }
                } else return 0;
            }
            printf("Quit!\n");
            return 0;
        default:
            return -1;
    }
}

int main()
{
    if (init_global_vars()){
        return -1;
    }

    // Command processing
    int command_code = INVALID_COMMAND;
    while (command_code != QUIT)
    {
        char command[256];
        printf("$ ");
        scanf("%s", command);
        command_code = parse_command(command);
        if (call_command(command_code)){
            printf("Invalid command, please entry a valid command!\n");
        }
    }
    return 0;
}