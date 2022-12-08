#include "lib.h"
#include <stdio.h>
#include <stdlib.h>

void copy_freestats(struct copyjob_stats *stats)
{
    printf("Free stats memorty!\n");
    free(stats);
};