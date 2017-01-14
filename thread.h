#ifndef THREAD_H_INCLUDED
#define THREAD_H_INCLUDED

typedef void * (* ThreadFunction)(void *);

typedef struct Thread *Thread;

typedef enum {
    SchedulingClassRealtime
} SchedulingClass;

Thread
Thread_create(ThreadFunction f, void *arg);

int
Thread_join(Thread t);

int
Thread_set_scheduling_class(Thread t, SchedulingClass class);

void
Thread_free(Thread *t);

#endif
