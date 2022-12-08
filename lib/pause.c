#include "lib.h"
#include <stdio.h>
#include <stdlib.h>

int copy_pause(copyjob_t job)
{
    printf("Pause JOB %d!\n", job);
    return 0;
};