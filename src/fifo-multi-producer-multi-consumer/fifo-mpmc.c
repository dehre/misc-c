#include "common.h"
#include "common_threads.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define THREADS_COUNT 3

//==================================================================================================
// FIFO QUEUE
//==================================================================================================

#define FIFO_SIZE 3
int FIFO[FIFO_SIZE];
int *PutPtr;
int *GetPtr;

pthread_mutex_t FIFOMutex;
semaphore_t *FIFOCurrentSize;
semaphore_t *FIFORoomLeft;

void FIFO_Init(void)
{
    PutPtr = GetPtr = &(FIFO[0]);
    Pthread_mutex_init(&FIFOMutex);
    Sem_init(FIFOCurrentSize, 0);
    Sem_init(FIFORoomLeft, FIFO_SIZE);
}

void FIFO_Put(int data)
{
    Sem_wait(FIFORoomLeft);
    Pthread_mutex_lock(&FIFOMutex);

    *PutPtr = data;
    PutPtr++;
    if (PutPtr == &(FIFO[FIFO_SIZE]))
    {
        PutPtr = &(FIFO[0]);
    }

    Pthread_mutex_unlock(&FIFOMutex);
    Sem_post(FIFOCurrentSize);
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
    Sem_post(FIFORoomLeft);
    return data;
}

void FIFO_Destroy(void)
{
    Pthread_mutex_destroy(&FIFOMutex);
    Sem_destroy(FIFOCurrentSize);
    Sem_destroy(FIFORoomLeft);
}

//==================================================================================================
// PRODUCER AND CONSUMER THREADS
//==================================================================================================

/*
 * Threads `producer` and `consumer` are passed their argument by value.
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
    int producer_id = (int)arg;
    for (size_t i = 0; i < 3; i++)
    {
        int data = rand() % 100;
        printf("Producer %d: put %d\n", producer_id, data);
        FIFO_Put(data);
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
    pthread_t producer_threads[THREADS_COUNT];
    pthread_t consumer_threads[THREADS_COUNT];
    for (size_t i = 0; i < THREADS_COUNT; i++)
    {
        Pthread_create(producer_threads + i, NULL, producer, (void *)i);
        Pthread_create(consumer_threads + i, NULL, consumer, (void *)i);
    }

    for (size_t i = 0; i < THREADS_COUNT; i++)
    {
        Pthread_join(producer_threads[i], NULL);
    }
    sleep(1); /* Wait for consumers to get all the data */
    FIFO_Destroy();
    return 0;
}
