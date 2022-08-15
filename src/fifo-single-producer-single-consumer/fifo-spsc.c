#include "common.h"
#include "common_semaphores.h"
#include "common_threads.h"
#include <stdio.h>
#include <unistd.h>

//==================================================================================================
// FIFO QUEUE
//==================================================================================================

#define FIFO_SIZE 3
int FIFO[FIFO_SIZE];
int *PutPtr;
int *GetPtr;

semaphore_t FIFOCurrentSize;

typedef enum [[nodiscard]]
{
    FIFO_SUCCESS = 0,
    FIFO_FAIL,
} FIFO_Rc;

void FIFO_Init(void)
{
    PutPtr = GetPtr = &(FIFO[0]);
    Sem_init(&FIFOCurrentSize, 0);
}

FIFO_Rc FIFO_Put(int data)
{
    int current_size;
    Sem_getvalue(&FIFOCurrentSize, &current_size);
    if (current_size == FIFO_SIZE)
    {
        return FIFO_FAIL;
    }

    *PutPtr = data;
    PutPtr++;
    if (PutPtr == &(FIFO[FIFO_SIZE]))
    {
        PutPtr = &(FIFO[0]);
    }

    Sem_post(&FIFOCurrentSize);
    return FIFO_SUCCESS;
}

int FIFO_Get(void)
{
    Sem_wait(&FIFOCurrentSize);

    int data = *GetPtr;
    GetPtr++;
    if (GetPtr == &(FIFO[FIFO_SIZE]))
    {
        GetPtr = &(FIFO[0]);
    }
    return data;
}

void FIFO_Destroy(void)
{
    Sem_destroy(&FIFOCurrentSize);
}

//==================================================================================================
// PRODUCER AND CONSUMER THREADS
//==================================================================================================

void *producer(void *arg)
{
    (void)arg;
    for (size_t i = 0; i < 8; i++)
    {
        int data = rand() % 100;
        FIFO_Rc rc = FIFO_Put(data);
        rc != FIFO_SUCCESS ? printf("Producer: DATA LOST %d\n", data) : printf("Producer: put %d\n", data);
    }
    return NULL;
}

void *consumer(void *arg)
{
    (void)arg;
    while (1)
    {
        int data = FIFO_Get();
        printf("Consumer: get %d\n", data);
    }
    return NULL;
}

//==================================================================================================
// MAIN
//==================================================================================================

int main(void)
{
    Sranddev();
    FIFO_Init();
    pthread_t producer_thread;
    pthread_t consumer_thread;
    Pthread_create(&producer_thread, NULL, producer, NULL);
    Pthread_create(&consumer_thread, NULL, consumer, NULL);

    Pthread_join(producer_thread, NULL);
    sleep(1); /* Wait for consumers to get all the data */
    FIFO_Destroy();
    return 0;
}
