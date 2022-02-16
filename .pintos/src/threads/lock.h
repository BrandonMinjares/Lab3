#ifndef THREADS_LOCK_H
#define THREADS_LOCK_H

#include <list.h>
#include <stdbool.h>
#include "threads/semaphore.h"

/* A Lock */
struct lock {
  struct thread *holder;      /* Thread holding lock */
  struct semaphore semaphore; /* Binary semaphore controlling access. */
};

void lock_init (struct lock *);
void lock_acquire (struct lock *);
void lock_release (struct lock *);
bool lock_held_by_current_thread (const struct lock *);

#endif /* threads/LOCK.h */
