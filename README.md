# Miscellaneous C

A place to put tiny snippets of C code that don't deserve a repository on their own.

<img src="readme_assets/sketch-astronaut.jpg" width=90% height=90%>

## Moore finite-state machine (fsm) - [code](https://github.com/dehre/misc-c/tree/main/src/finite-state-machine)

A simple example of a Moore finite-state machine for a pedestrian traffic light.  

## Multi-producer Multi-consumer FIFO Queue (fifo-mpmc) - [code](https://github.com/dehre/misc-c/tree/main/src/fifo-multi-producer-multi-consumer)

A FIFO queue used to pass data from multiple producer main threads to multiple consumer main threads.

## Single-producer Multi-consumer FIFO Queue (fifo-spmc) - [code](https://github.com/dehre/misc-c/tree/main/src/fifo-single-producer-multi-consumer)

A FIFO queue used to pass data from a single producer event thread to multiple consumer main threads.  
Unlike the multi-producer-multi-consumer-queue, the producer is assumed to be an event thread (e.g. ISR) that cannot block or spin
on a semaphore when the queue is full: if that happens, the data point is simply lost.

## Single-producer Single-consumer FIFO Queue (fifo-spsc) - [code](https://github.com/dehre/misc-c/tree/main/src/fifo-single-producer-single-consumer)

A FIFO queue used to pass data from a single producer event thread to a single consumer main thread.  
Unlike the single-producer-multi-consumer-queue, having a single thread that can call `FIFO_Get` means there are no critical sections
and the mutex can be removed.

## ASIDE: Semaphores on MacOS :(

MacOS apparently has no support for unnamed semaphores.  
Still, in order to comply with POSIX, it declares the functions, marks them as deprecated, and lets them return `-1` with `errno` set to `ENOSYS` (function not implemented). Wow.

```c
// <sys/semaphore.h>
int sem_init(sem_t *, int, unsigned int) __deprecated;
int sem_getvalue(sem_t * __restrict, int * __restrict) __deprecated;
int sem_destroy(sem_t *) __deprecated;
```

Furthermore, [named semaphores](https://man7.org/linux/man-pages/man7/sem_overview.7.html) (the only ones available on MacOS) exist beyond the end of the process and could potentially affect other processes that don't belong to you, so creating and deleting them requires root permissions, that is, running the executable with `sudo`.

Overall, supporting both Linux and MacOS turned out to be a bit of an headache and required [some bits of preprocessor magic](https://github.com/dehre/misc-c/blob/main/src/include/common_threads.h).

## ASIDE: The astronaut...

The [nice cartoon sketch](https://pixabay.com/illustrations/sketch-cartoon-space-set-3045125/) is offered by Pixabay users under their [generous free license](https://pixabay.com/service/license/).
