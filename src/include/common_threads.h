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
 * Furthermore in MacOS the fn `sem_getvalue` is deprecated,
 *   so we have to keep track of the semaphore's value on our own.
 */

#ifdef __linux__
typedef sem_t semaphore_t;
#define Sem_init(sem, value)                                                                                           \
    {                                                                                                                  \
        static semaphore_t _##sem;                                                                                     \
        sem = &(_##sem);                                                                                               \
        assert(sem_init(sem, 0, value) == 0);                                                                          \
    }
#define Sem_wait(sem) assert(sem_wait(sem) == 0)
#define Sem_post(sem) assert(sem_post(sem) == 0)
#define Sem_getvalue(sem)                                                                                              \
    ({                                                                                                                 \
        int val;                                                                                                       \
        assert(sem_getvalue(sem, &val) == 0);                                                                          \
        val;                                                                                                           \
    })
#define Sem_destroy(sem) assert(sem_destroy(sem) == 0)

#elif __APPLE__
#include <stdatomic.h>
typedef struct
{
    sem_t *data;
    atomic_int val;
} semaphore_t;
#define Sem_init(sem, value)                                                                                           \
    {                                                                                                                  \
        static semaphore_t s;                                                                                          \
        s.data = sem_open(#sem, O_CREAT, O_RDWR, value);                                                               \
        assert(s.data != SEM_FAILED);                                                                                  \
        s.val = value;                                                                                                 \
        (sem) = &s;                                                                                                    \
    }
#define Sem_wait(sem)                                                                                                  \
    {                                                                                                                  \
        assert(sem_wait((sem)->data) == 0);                                                                            \
        (sem)->val -= 1;                                                                                               \
    }
#define Sem_post(sem)                                                                                                  \
    {                                                                                                                  \
        (sem)->val += 1;                                                                                               \
        assert(sem_post((sem)->data) == 0);                                                                            \
    }
#define Sem_getvalue(sem) ((sem)->val)
#define Sem_destroy(sem) assert(sem_unlink(#sem) == 0)

#else
#error "Platform not supported"
#endif
