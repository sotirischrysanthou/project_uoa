#ifndef _JOBSCHEDULER_
#define _JOBSCHEDULER_

#pragma once

#include "List.h"
#include <pthread.h>

struct job
{
    

};

struct jobScheduler
{
    int thread_count; // number of execution threads
    List queue;              // a queue that holds submitted jobs / tasks
    pthread_t *tids;      // execution threads
    pthread_mutex_t mutex;
    pthread_cond_t cond;

    jobScheduler(int t_count);
};

#endif