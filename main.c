// Damon file
#include "config.h"
#include "lib/lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>

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
sem_t semaphore;

// Input command parser
int parse_command(char *command)
{
    if (strcmp(command, "cp") == 0) return CREATE;
    else if (strcmp(command, "ls") == 0) return LIST;
    else if (strcmp(command, "quit") == 0 || strcmp(command, "q") == 0) return QUIT;
    else if (strcmp(command, "cancel") == 0) return CANCEL;
    else if (strcmp(command, "pause") == 0) return PAUSE;
    else if (strcmp(command, "resume") == 0) return RESUME;
    else if (strcmp(command, "stats") == 0) return STATS;
    return INVALID_COMMAND;
}

int call_command(int code)
{
    switch (code){
        case CREATE:
            // Create new copy JOB
            char src[256], dst[256];

            scanf("%s", src);
            scanf("%s", dst);

            copyjob_t job_id = copy_createjob(src, dst);
            printf("Copy job with ID=%d has started!\n", job_id);
            return 0;
        case LIST:
            // List all JOBS with their state (IN PROGRESS, WAITING, PAUSED)
            printf("List jobs\n");
            return 0;
        
        case PAUSE:
            // Pause JOB execution
            // copyjob_t job_id;
            // scanf("%d", job_id);
            // copy_pause(job_id);
            printf("Pause a JOB\n");
            return 0;
        case RESUME:
            // Resume JOB execution
            printf("Resume a JOB\n");
            return 0;
        case STATS:
            // Optain JOB stats and progress
            printf("JOB stats\n");
            return 0;
        case CANCEL:
            // Cancel JOB execution
            printf("Cancel a JOB\n");
            return 0;
        case QUIT:
            // Quit DAEMON execution
            printf("Quit!\n");
            return 0;
        default:
            return -1;
    }
}

int main()
{
    // Init job_mutexes
    for (int i = 0; i < MAX_JOBS; i++)
        if (pthread_mutex_init(&job_mutexes[i], NULL))
        {
            perror(NULL);
            return errno;
        }
    
    // Init semaphore
    if (sem_init(&semaphore, 0, MAX_THREADS))
    {
        perror(NULL);
        return errno;
    }

    // Command processing
    int command_code = INVALID_COMMAND;
    while (command_code != QUIT)
    {
        char command[256];
        scanf("%s", command);
        command_code = parse_command(command);
        if (call_command(command_code)){
            printf("Invalid command, please entry a valid command!");
        }
    }
    return 0;
}