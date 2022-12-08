// Damon file
#include "lib.h"
#include "config.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>

// Define commands
#define QUIT 0
#define CREATE 1
#define CANCEL 2

int parse_command(char *command)
{
    if (strcmp(command, "cp") == 0)
    {
        printf("Create\n");
        return CREATE;
    }
    if (strcmp(command, "quit") == 0)
    {
        printf("Quit\n");
        return QUIT;
    }
    if (strcmp(command, "cancel") == 0)
    {
        printf("Cancel\n");
        return CANCEL;
    }
}

int main()
{
    // Init job_mutexes
    pthread_mutex_t job_mutexes[MAX_JOBS];
    for (int i = 0; i < MAX_JOBS; i++)
        if (pthread_mutex_init(&job_mutexes[i], NULL))
        {
            perror(NULL);
            return errno;
        }
    // Init semaphore
    sem_t semaphore;
    if (sem_init(&semaphore, 0, MAX_THREADS))
    {
        perror(NULL);
        return errno;
    }
    int code = -1;
    while (code != QUIT)
    {
        char command[256];
        scanf("%s", command);
        code = parse_command(command);
    }
    return 0;
}