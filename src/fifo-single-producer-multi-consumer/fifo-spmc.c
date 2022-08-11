#include "common.h"
#include "common_threads.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define CONSUMER_THREADS_COUNT 3

//==================================================================================================
// FIFO QUEUE
//==================================================================================================

#define FIFO_SIZE 3
int FIFO[FIFO_SIZE];
int *PutPtr;
int *GetPtr;

pthread_mutex_t FIFOMutex;
semaphore_t *FIFOCurrentSize;

typedef enum [[nodiscard]]
{
    FIFO_SUCCESS = 0,
    FIFO_FAIL,
} FIFO_Rc;

void FIFO_Init(void)
{
    PutPtr = GetPtr = &(FIFO[0]);
    Pthread_mutex_init(&FIFOMutex);
    Sem_init(FIFOCurrentSize, 0);
}

FIFO_Rc FIFO_Put(int data)
{
    if (Sem_getvalue(FIFOCurrentSize) == FIFO_SIZE)
    {
        return FIFO_FAIL;
    }

    *PutPtr = data;
    PutPtr++;
    if (PutPtr == &(FIFO[FIFO_SIZE]))
    {
        PutPtr = &(FIFO[0]);
    }

    Sem_post(FIFOCurrentSize);
    return FIFO_SUCCESS;
}

int FIFO_Get(void)
{
    Sem_wait(FIFOCurrentSize);
    Pthread_mutex_lock(&FIFOMutex);

    int data = *GetPtr;
    GetPtr++;
    if (GetPtr == &(FIFO[FIFO_SIZE]))
    {
        GetPtr = &(FIFO[0]);
    }

    Pthread_mutex_unlock(&FIFOMutex);
    return data;
}

void FIFO_Destroy(void)
{
    Pthread_mutex_destroy(&FIFOMutex);
    Sem_destroy(FIFOCurrentSize);
}

//==================================================================================================
// PRODUCER AND CONSUMER THREADS
//==================================================================================================

/*
 * Thread `consumer` is passed its argument by value.
 * It's a bit wild, but does the job.
 */
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
#elif __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wvoid-pointer-to-int-cast"
#endif

void *producer(void *arg)
{
    (void)arg;
    for (size_t i = 0; i < 15; i++)
    {
        int data = rand() % 100;
        FIFO_Rc rc = FIFO_Put(data);
        rc != FIFO_SUCCESS ? printf("Producer: DATA LOST %d\n", data) : printf("Producer: put %d\n", data);
    }
    return NULL;
}

void *consumer(void *arg)
{
    int consumer_id = (int)arg;
    while (1)
    {
        int data = FIFO_Get();
        printf("Consumer %d: get %d\n", consumer_id, data);
    }
    return NULL;
}

#ifdef __GNUC__
#pragma GCC diagnostic pop
#elif __clang__
#pragma clang diagnostic pop
#endif

//==================================================================================================
// MAIN
//==================================================================================================

int main(void)
{
    if (getuid() != 0)
    {
        /* On MacOS `Sem_init` creates a named semaphore, which needs root privileges to be deleted */
        fprintf(stderr, "please run the program as root\n");
        exit(1);
    }

    FIFO_Init();
    Sranddev();
    pthread_t producer_thread;
    pthread_t consumer_threads[CONSUMER_THREADS_COUNT];
    Pthread_create(&producer_thread, NULL, producer, NULL);
    for (size_t i = 0; i < CONSUMER_THREADS_COUNT; i++)
    {
        Pthread_create(consumer_threads + i, NULL, consumer, (void *)i);
    }

    Pthread_join(producer_thread, NULL);
    sleep(1); /* Ideally consumers would run indefinitely */
    FIFO_Destroy();
    return 0;
}
