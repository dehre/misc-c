#pragma once

#include <assert.h>
#include <pthread.h>

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
