#pragma once

#include "common_threads.h"

/*
 * Unnamed semaphores are deprecated in MacOS, so we rely on our own implementation.
 */

#ifdef __linux__
#include <semaphore.h>
typedef sem_t semaphore_t;
#elif __APPLE__
typedef struct semaphore_s
{
    int val;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} semaphore_t;
int sem_init(semaphore_t *sem, int pshared, int val);
int sem_wait(semaphore_t *sem);
int sem_post(semaphore_t *sem);
int sem_getvalue(semaphore_t *sem, int *val);
int sem_destroy(semaphore_t *sem);
#else
#error "Platform not supported"
#endif

#define Sem_init(sem, value) assert(sem_init(sem, 0, value) == 0)
#define Sem_wait(sem) assert(sem_wait(sem) == 0)
#define Sem_post(sem) assert(sem_post(sem) == 0)
#define Sem_getvalue(sem, value_ptr) assert(sem_getvalue(sem, value_ptr) == 0)
#define Sem_destroy(sem) assert(sem_destroy(sem) == 0)
