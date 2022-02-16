#ifndef THREADS_BARRIER_H
#define THREADS_BARRIER_H

#define barrier() asm volatile ("" : : : "memory")

#endif // THREADS_BARRIER_H