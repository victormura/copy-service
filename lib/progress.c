#include "lib.h"
#include <stdio.h>
#include <stdlib.h>

int copy_progress(copyjob_t job){
    printf("Progress JOB %d\n", job);
    return 0;
};