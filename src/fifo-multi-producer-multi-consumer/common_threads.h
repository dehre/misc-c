#pragma once

#include <assert.h>
#include <pthread.h>
#include <semaphore.h>

#define Pthread_create(thread, attr, start_routine, arg) assert(pthread_create(thread, attr, start_routine, arg) == 0)
#define Pthread_join(thread, value_ptr) assert(pthread_join(thread, value_ptr) == 0)

#define Pthread_mutex_init(m) assert(pthread_mutex_init(m, NULL) == 0)
#define Pthread_mutex_lock(m) assert(pthread_mutex_lock(m) == 0)
#define Pthread_mutex_unlock(m) assert(pthread_mutex_unlock(m) == 0)
#define Pthread_mutex_destroy(m) assert(pthread_mutex_destroy(m) == 0)

#define Pthread_cond_init(cond) assert(pthread_cond_init(cond, NULL) == 0)
#define Pthread_cond_signal(cond) assert(pthread_cond_signal(cond) == 0)
#define Pthread_cond_wait(cond, mutex) assert(pthread_cond_wait(cond, mutex) == 0)
#define Pthread_cond_destroy(cond) assert(pthread_cond_destroy(cond) == 0)

/*
 * In Linux the fn `sem_init` creates an unnamed semaphore by initializing an existing blob of memory.
 * In MacOS the fn `sem_init` is deprecated in favor of `sem_open`,
 *   which creates a named semaphore by allocating memory on the heap.
 */

#ifdef __linux__
#define Sem_init(sem, value)                                                                                           \
    {                                                                                                                  \
        static sem_t _##sem;                                                                                           \
        sem = &(_##sem);                                                                                               \
        assert(sem_init(sem, 0, value) == 0);                                                                          \
    }
#define Sem_wait(sem) assert(sem_wait(sem) == 0)
#define Sem_post(sem) assert(sem_post(sem) == 0)
#define Sem_destroy(sem) assert(sem_destroy(sem) == 0)
#else /* MacOS */
#define Sem_init(sem, value)                                                                                           \
    {                                                                                                                  \
        (sem) = sem_open(#sem, O_CREAT, O_RDWR, value);                                                                \
        assert((sem) != SEM_FAILED);                                                                                   \
    }
#define Sem_wait(sem) assert(sem_wait(sem) == 0)
#define Sem_post(sem) assert(sem_post(sem) == 0)
#define Sem_destroy(sem) assert(sem_unlink(#sem) == 0)
#endif
