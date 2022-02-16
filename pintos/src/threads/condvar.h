#ifndef THREADS_CONDVAR_H
#define THREADS_CONDVAR_H

#include <list.h>
#include <stdbool.h>
#include "threads/lock.h"

/* Condition variable. */
struct condvar {
  struct list waiters;   /* List of semaphores with waiting threads. */
};

void condvar_init (struct condvar *);
void condvar_wait (struct condvar *, struct lock *);
void condvar_signal (struct condvar *, struct lock *);
void condvar_broadcast (struct condvar *, struct lock *);

#endif /* threads/CONDVAR.h */
