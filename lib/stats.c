#include "lib.h"
#include <stdio.h>
#include <stdlib.h>

int copy_stats(copyjob_t job, copyjob_stats *stats)
{
    printf("Stats JOB %d!\n", job);
    return 0;
};

void copy_freestats(copyjob_stats *stats)
{
    printf("Free stats memorty!\n");
    free(stats->dst);
    free(stats->src);
    free(stats);
};