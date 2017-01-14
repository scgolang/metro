// +build cgo
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "mem.h"
#include "thread.h"

struct Thread {
    pthread_t threadId;
};

Thread
Thread_create(ThreadFunction f, void *arg)
{
    Thread t;
    NEW(t);
    int error = pthread_create(&t->threadId, NULL, f, arg);
    if (error) {
        fprintf(stderr, "Could not create thread (Error %d)\n", error);
        exit(EXIT_FAILURE);
    }
    return t;
}

int
Thread_set_scheduling_class(Thread t, SchedulingClass class)
{
    const struct sched_param sp;
    
    switch (class) {
    case SchedulingClassRealtime:
        pthread_setschedparam(t->threadId, SCHED_FIFO, &sp);
        // error if sp.sched_priority is not SCHED_FIFO?
        break;
    }
    return 0;
}

int
Thread_join(Thread t)
{
    assert(t);
    return pthread_join(t->threadId, NULL);
}

void
Thread_free(Thread *t)
{
    assert(t && *t);
    FREE(*t);
}
