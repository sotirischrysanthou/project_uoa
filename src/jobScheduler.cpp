#include "jobScheduler.h"

/* based on place function from the class slides */
void place(List pool, pthread_mutex_t *mtx, job *data)
{
    pthread_mutex_lock(mtx);
    // while (pool->count >= pool->size)
    // {
    //     // printf(" >> Found Buffer Full\n");
    //     pthread_cond_wait(&cond_nonfull, &mtx);
    // }
    // pool->end = (pool->end + 1) % pool->size;
    // pool->data[pool->end] = data;
    // pool->count++;
    pthread_mutex_unlock(mtx);
    // pthread_cond_signal(&cond_nonempty);
}

/* based on obtain function from the class slides */
job *obtain(List pool, pthread_mutex_t *mtx)
{
    job*j;
    pthread_mutex_lock(mtx);
    // while (pool->count <= 0)
    // {
    //     // printf(" >> Found Buffer Empty\n");
    //     pthread_cond_wait(&cond_nonempty, &mtx);
    // }
    // data = pool->data[pool->start];
    // pool->start = (pool->start + 1) % pool->size;
    // pool->count--;
    pthread_mutex_unlock(mtx);
    // pthread_cond_signal(&cond_nonfull);
    return j;
}

jobScheduler::jobScheduler(int t_count) : thread_count(t_count)
{
    queue = new list(NULL);
    tids = new pthread_t[t_count];
}

void jobScheduler::submit_job(job *j)
{
    queue->list_insert_next(queue->list_last(), j);
}

void jobScheduler::execute_jobs()
{
}

void jobScheduler::wait_all()
{
    for (int i=0;i<thread_count;i++)
    {
        pthread_join(tids[i], NULL);
    }
}

jobScheduler::~jobScheduler()
{
    delete[] tids;
}