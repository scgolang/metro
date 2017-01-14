// +build cgo
#include <assert.h>
#include <mach/mach.h>
#include <mach/mach_time.h>
#include <pthread.h>
#include <stdint.h>
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
    const uint64_t NANOS_PER_MSEC = 1000000ULL;
    mach_timebase_info_data_t timebase_info;
    double recip, clock2abs;
    thread_time_constraint_policy_data_t policy;
    int kr;

    switch (class) {
    case SchedulingClassRealtime:
        // Swiped from https://developer.apple.com/library/ios/technotes/tn2169/_index.html
        // on Wed Mar 18 14:33:46 UTC 2015
        // [bps]
        mach_timebase_info(&timebase_info);
 
        recip = (double)timebase_info.denom / (double)timebase_info.numer;
        clock2abs = recip * NANOS_PER_MSEC;
 
        policy.period      = 0;
        policy.computation = (uint32_t)(5 * clock2abs); // 5 ms of work
        policy.constraint  = (uint32_t)(10 * clock2abs);
        policy.preemptible = FALSE;
 
        kr = thread_policy_set(pthread_mach_thread_np(t->threadId),
                               THREAD_TIME_CONSTRAINT_POLICY,
                               (thread_policy_t)&policy,
                               THREAD_TIME_CONSTRAINT_POLICY_COUNT);

        if (kr != KERN_SUCCESS) {
            return 1;
        }
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
