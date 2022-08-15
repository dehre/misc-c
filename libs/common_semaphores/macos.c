#ifdef __APPLE__

#include "common_semaphores.h"

#define sem_val (sem->val)
#define sem_mutex (sem->mutex)
#define sem_cond (sem->cond)

int sem_init(semaphore_t *sem, int pshared, int val) // NOLINT
{
    (void)pshared;
    sem_val = val;
    sem_mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    sem_cond = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
    return 0;
}

int sem_wait(semaphore_t *sem)
{
    Pthread_mutex_lock(&sem_mutex);
    sem_val--;
    while (sem_val < 0)
    {
        Pthread_cond_wait(&sem_cond, &sem_mutex);
    }
    Pthread_mutex_unlock(&sem_mutex);
    return 0;
}

int sem_post(semaphore_t *sem)
{
    Pthread_mutex_lock(&sem_mutex);
    sem_val++;
    if (sem_val <= 0)
    {
        Pthread_cond_signal(&sem_cond);
    }
    Pthread_mutex_unlock(&sem_mutex);
    return 0;
}

int sem_getvalue(semaphore_t *sem, int *val)
{
    *val = sem_val;
    return 0;
}

int sem_destroy(semaphore_t *sem)
{
    Pthread_mutex_destroy(&sem_mutex);
    Pthread_cond_destroy(&sem_cond);
    return 0;
}

#endif // __APPLE_
