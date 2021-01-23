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
    int thread_count;                       // number of execution threads
    pool_t *pool;                           // a pool that holds submitted jobs / tasks
    pthread_t *tids;                        // execution threads
    pthread_mutex_t mutex;                  // mutex for critical code
    pthread_mutex_t mtx_running_threads;    // mutex for updating running_threads number
    pthread_mutex_t list_mutex;             // mutex for accessing list
    pthread_cond_t cond_nonfull;            // condition for when the pool is full
    pthread_cond_t cond_nonempty;           // condition for when the pool is empty
    pthread_cond_t cond_running;            // condition for when there are threads running
    int running_threads;

    thread_args *t_args;
    List return_values;                     // a list for storing the return value of each thread's run() function
    
public:
    /* the constructor initializes all mutexes and threads and starts the threads with execute_jobs() */
    jobScheduler(int t_count, int p_size);
    /* submit a job into the pool/buffer */
    void submit_job(job *j);
    /* start all threads to execute their jobs (this function creates the threads) */
    void execute_jobs();
    /* get the list of returned values from all run() functions */
    List get_return_values();
    /* wait for the threads to finish their jobs */
    void wait_all();
    /* terminate the threads (this funcion runs pthread_join()) */
    void terminate_all();
    
    ~jobScheduler();
};



void *thread_main(void *args);

#endif

