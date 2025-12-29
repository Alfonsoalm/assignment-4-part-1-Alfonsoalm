
#include "threading.h"
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define DEBUG_LOG(msg, ...)
#define ERROR_LOG(msg, ...) printf("threading ERROR: " msg "\n", ##__VA_ARGS__)

void *threadfunc(void *thread_param)
{
    if (thread_param == NULL)
    {
        ERROR_LOG("thread_param is NULL");
        return NULL;
    }

    struct thread_data *td = (struct thread_data *)thread_param;
    td->thread_complete_success = false;

    if (td->wait_to_obtain_ms > 0)
        usleep((unsigned int)(td->wait_to_obtain_ms * 1000));

    int rc = pthread_mutex_lock(td->mutex);
    if (rc != 0)
    {
        ERROR_LOG("pthread_mutex_lock failed (rc=%d)", rc);
        return thread_param;
    }

    if (td->wait_to_release_ms > 0)
        usleep((unsigned int)(td->wait_to_release_ms * 1000));

    rc = pthread_mutex_unlock(td->mutex);
    if (rc != 0)
    {
        ERROR_LOG("pthread_mutex_unlock failed (rc=%d)", rc);
        return thread_param;
    }

    td->thread_complete_success = true;

    return thread_param;
}

bool start_thread_obtaining_mutex(pthread_t *thread,
                                  pthread_mutex_t *mutex,
                                  int wait_to_obtain_ms,
                                  int wait_to_release_ms)
{
    if (thread == NULL || mutex == NULL)
    {
        ERROR_LOG("start_thread_obtaining_mutex: thread or mutex is NULL");
        return false;
    }

    struct thread_data *td = (struct thread_data *)malloc(sizeof(struct thread_data));
    if (td == NULL)
    {
        ERROR_LOG("malloc thread_data failed");
        return false;
    }

    td->mutex = mutex;
    td->wait_to_obtain_ms = wait_to_obtain_ms;
    td->wait_to_release_ms = wait_to_release_ms;
    td->thread_complete_success = false;

    int rc = pthread_create(thread, NULL, threadfunc, (void *)td);
    if (rc != 0)
    {
        ERROR_LOG("pthread_create failed (rc=%d)", rc);
        free(td);
        return false;
    }
    return true;
}
