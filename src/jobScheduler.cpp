#include "jobScheduler.h"

jobScheduler::jobScheduler(int t_count) : thread_count(t_count)
{
    queue = new list(NULL);
}