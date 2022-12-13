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
#include <linux/limits.h>

// Define commands
#define INVALID_COMMAND -1
#define QUIT 0
#define CREATE 1
#define CANCEL 2
#define PAUSE 3
#define RESUME 4
#define STATS 5
#define LIST 6
#define HELP 7
#define CONTINUE 8

#define MAX_INPUT_LENGTH 2 * PATH_MAX + 256

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
    else if (strcmp(command, "help") == 0) return HELP;
    return INVALID_COMMAND;
}

int call_command(char input[])
{
    int code = parse_command(strtok(input, " ")); //" " -> "\0"
    copyjob_t job_id;
    char buf[number_of_digits(MAX_JOBS)];
    int max_argc = 2;
    char ** argv = (char**)malloc(sizeof(char*)*max_argc);

    for (int i = 0; i< max_argc; i++){
        argv[i] = (char*)malloc(sizeof(char)*PATH_MAX);
        argv[i] = strtok(NULL, " ");
    }
    int number_of_arguments = 0;
    // Check args
    switch (code)
    {
        case CREATE:
            number_of_arguments = 2;
            break;
        case STATS:
        case RESUME:
        case PAUSE:
        case CANCEL:
            number_of_arguments = 1;
            break;
        default:
            break;
    }
    for (int i = 0; i < number_of_arguments; i++){
        if (argv[i] == NULL) return -1;
    };

    switch (code){
        case CREATE: ;
            // Create new copy JOB
            job_id = copy_createjob(argv[0], argv[1]);
            if (job_id == -1) {
                printf("Couldn't create a new job!\n");
                break;
            }
            printf("Copy job with ID=%d has started!\n", job_id);
            break;
        case LIST:
            // List all JOBS with their state (IN PROGRESS, WAITING, PAUSED)
            copy_listjobs();
            break;
        
        case PAUSE: 
            // Pause JOB execution
            job_id = atoi(argv[0]); 
            copy_pause(job_id);
            printf("Job %d paused!\n", job_id);
            break;

        case RESUME:
            // Resume JOB execution
            job_id = atoi(argv[0]); 
            copy_resume(job_id);
            printf("Job %d resumed!\n", job_id);
            break;

        case STATS:
            // Optain JOB stats and progress
            job_id = atoi(argv[0]); 
            copy_progress(job_id);
            break;

        case CANCEL:
            job_id = atoi(argv[0]); 
            copy_cancel(job_id);
            printf("Job %d canceled!\n", job_id);
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
                char response[3];
                if(!fgets(response, 3, stdin)) return -1;
                response[strcspn(response, "\n")] = 0;
                if (strcmp(response, "c") == 0){
                    for (int i = 0; i < MAX_THREADS; i++){
                        if (jobs_stats[i].state != AVAILABLE) copy_cancel(i);
                    }
                } 
                else if ((strcmp(response, "w") == 0)) {
                    // Wait until all jobs will finish
                    for (int i = 0; i < MAX_JOBS; i++){
                        if (jobs_stats[i].state == PAUSED) copy_resume(i);
                    }
                    printf("Waiting");
                    fflush(NULL);
                    for (int i = 0; i < MAX_THREADS; i++){
                        if (i % (MAX_THREADS/5) == 0) printf(".");
                        fflush(NULL);
                        sem_wait(&semaphore);
                    }
                    printf("\n");
                } else return CONTINUE;
            }
            printf("Quit!\n");
            break;

        case HELP: ;
            printf("cp -- Start a copy process\ncp source_file target_file\n\n");
            printf("ls -- List information about every copy process\n\n");
            printf("quit -- End all processes\n\n");
            printf("cancel -- Cancel an ongoing copy process\ncancel target_job\n\n");
            printf("pause -- Halt an ongoing process until resumed\npause target_job\n\n");
            printf("resume -- Resume a paused process\nresume target_job\n\n");
            printf("stats -- Get information about an ongoing process\nstats target_job\n");
            break;
        default:
            return -1;
    }
    return code;
}

int main()
{
    if (init_global_vars()){
        return -1;
    }
    char command[MAX_INPUT_LENGTH];
    // Command processing
    int command_code = INVALID_COMMAND;
    while (command_code != QUIT)
    {
        printf("$ ");
        while(fgets(command, MAX_INPUT_LENGTH, stdin) == NULL);
        command[strcspn(command, "\n")] = 0;
        command_code = call_command(command);
        if (command_code < 0){
            printf("Invalid command, please entry a valid command!\n");
        }
    }
    return 0;
}