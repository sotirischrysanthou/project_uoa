#include "jobScheduler.h"

/* based on place function from the class slides */
void place(pool_t *pool,
           pthread_mutex_t *mtx,
           pthread_cond_t *cond_nonfull,
           pthread_cond_t *cond_nonempty,
           job *data)
{
    pthread_mutex_lock(mtx);
    while (pool->count >= pool->size)
    {
        // printf(" >> Found Buffer Full\n");
        pthread_cond_wait(cond_nonfull, mtx);
    }
    pool->end = (pool->end + 1) % pool->size;
    pool->jobs[pool->end] = data;
    pool->count++;
    pthread_mutex_unlock(mtx);
    pthread_cond_signal(cond_nonempty);
}

/* based on obtain function from the class slides */
job *obtain(pool_t *pool,
            pthread_mutex_t *mtx,
            pthread_cond_t *cond_nonfull,
            pthread_cond_t *cond_nonempty)
{
    job *j;
    pthread_mutex_lock(mtx);
    while (pool->count <= 0)
    {
        // printf(" >> Found Buffer Empty\n");
        pthread_cond_wait(cond_nonempty, mtx);
    }
    j = pool->jobs[pool->start];
    pool->start = (pool->start + 1) % pool->size;
    pool->count--;
    pthread_mutex_unlock(mtx);
    pthread_cond_signal(cond_nonfull);
    return j;
}

pool_t::pool_t(int size)
{
    this->size = size;
    jobs = new Job[size];
}

pool_t::~pool_t()
{
    delete[] jobs;
}

jobScheduler::jobScheduler(int t_count, int p_size) : thread_count(t_count)
{
    pool = new pool_t(p_size);
    tids = new pthread_t[t_count];
    return_values = new list(NULL);

    // TODO : check initializations (espesially conditions)
    pthread_mutex_init(&mutex, 0);
    pthread_mutex_init(&list_mutex, 0);
    pthread_cond_init(&cond_nonempty, 0);
    pthread_cond_init(&cond_nonfull, 0);
}

void jobScheduler::submit_job(job *j)
{
    place(pool, &mutex, &cond_nonfull, &cond_nonempty, j);
}

void jobScheduler::execute_jobs()
{

    t_args = new thread_args;
    t_args->cond_nonempty = &cond_nonempty;
    t_args->cond_nonfull = &cond_nonfull;
    t_args->mutex = &mutex;
    t_args->list_mutex = &list_mutex;
    t_args->pool = pool;
    t_args->return_values = return_values;
    for (int i = 0; i < thread_count; i++)
    {
        pthread_create(&tids[i], 0, thread_main, t_args);
    }
}

List jobScheduler::get_return_values()
{
    pthread_mutex_lock(&list_mutex);
    return return_values;
    pthread_mutex_unlock(&list_mutex);
}

void jobScheduler::wait_all()
{
    for (int i = 0; i < thread_count; i++)
        place(pool, &mutex, &cond_nonfull, &cond_nonempty, NULL);

    for (int i = 0; i < thread_count; i++)
        pthread_join(tids[i], NULL);
}

jobScheduler::~jobScheduler()
{
    wait_all();
    delete[] tids;
    delete t_args;
}

void *thread_main(void *args)
{
    thread_args *args_ptr = (thread_args *)args;
    pool_t *pool = args_ptr->pool;
    pthread_mutex_t *mutex = args_ptr->mutex;
    pthread_mutex_t *list_mutex = args_ptr->list_mutex;
    pthread_cond_t *cond_nonfull = args_ptr->cond_nonfull;
    pthread_cond_t *cond_nonempty = args_ptr->cond_nonempty;
    List return_values = args_ptr->return_values;

    Job j;
    while (1)
    {
        j = obtain(pool, mutex, cond_nonfull, cond_nonempty);
        if (j == NULL)
            pthread_exit(0);
        else
        {
            pthread_mutex_lock(list_mutex);
            return_values->list_insert_next(NULL, j->run());
            pthread_mutex_unlock(list_mutex);
        }
    }
}
