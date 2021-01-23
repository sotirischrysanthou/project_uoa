#ifndef _JOBSCHEDULER_
#define _JOBSCHEDULER_

#pragma once

#include "List.h"
#include <pthread.h>

typedef class job *Job;

/* other job classes will extend
   this and have their arguments
   as variables to be used in run() */
class job
{
    public:
    virtual Pointer run()=0;
};

struct pool_t
{
    Job *jobs;
    int start = 0;
    int end = -1;
    int count = 0;
    int size;
    pool_t(int size);
    ~pool_t();
};

struct thread_args
{
    pool_t *pool;
    pthread_mutex_t *mutex;
    pthread_mutex_t *list_mutex;
    pthread_mutex_t *mtx_running_threads;
    pthread_cond_t *cond_nonfull;
    pthread_cond_t *cond_nonempty;
    pthread_cond_t *cond_running;
    List return_values;
    int *running_threads;
};

class jobScheduler
{
    int thread_count; // number of execution threads
    pool_t *pool;     // a pool that holds submitted jobs / tasks
    pthread_t *tids;  // execution threads
    pthread_mutex_t mutex;
    pthread_mutex_t mtx_running_threads;
    pthread_mutex_t list_mutex;
    pthread_cond_t cond_nonfull;
    pthread_cond_t cond_nonempty;
    pthread_cond_t cond_running;
    int running_threads;

    thread_args *t_args;
    List return_values;
    
public:
    jobScheduler(int t_count, int p_size);
    void submit_job(job *j);
    void execute_jobs();
    List get_return_values();
    void wait_all();

    void terminate_all();
    ~jobScheduler();
};



void *thread_main(void *args);

#endif

