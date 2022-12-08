#ifndef LIB_H_   /* Include guard */
#define LIB_H_

typedef int copyjob_t;

struct copyjob_stats {

};

copyjob_t copy_createjob(const char * src, const char * dst);

int copy_cancel(copyjob_t job);

int copy_pause(copyjob_t job);

int copy_stats(copyjob_t job, struct copyjob_stats * stats);

int copy_progress(copyjob_t job);

int copy_listjobs(struct copyjob_stats ** statslist, unsigned int * jobscount);

void copy_freestats(struct copyjob_stats * stats);

#endif // LIB_H_