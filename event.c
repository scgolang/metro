// +build cgo
#include <assert.h>
#include <pthread.h>
#include <stddef.h>
#include <time.h>

#include "event.h"
#include "mem.h"

typedef enum {
    MetroEventState_NotReady,
    MetroEventState_Ready
} MetroEventState;

struct MetroEvent {
    void *val;
    MetroEventState state;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

MetroEvent
MetroEvent_init()
{
    MetroEvent e;
    NEW(e);
    pthread_mutex_init(&e->mutex, NULL);
    pthread_cond_init(&e->cond, NULL);
    e->state = MetroEventState_NotReady;
    e->val = NULL;
    return e;
}

int
MetroEvent_lock(MetroEvent e)
{
    assert(e);
    return pthread_mutex_lock(&e->mutex);
}

int
MetroEvent_wait(MetroEvent e)
{
    assert(e);
    e->state = MetroEventState_NotReady;
    int result = pthread_cond_wait(&e->cond, &e->mutex);
    if (e->state != MetroEventState_Ready) {
        return MetroEvent_wait(e);
    } else {
        return result;
    }
}

int
MetroEvent_timedwait(MetroEvent e, long ns)
{
    assert(e);
    e->state = MetroEventState_NotReady;
    struct timespec time;
    time.tv_nsec = ns;
    int result = pthread_cond_timedwait(&e->cond, &e->mutex, &time);
    if (e->state != MetroEventState_Ready) {
        return MetroEvent_timedwait(e, ns);
    } else {
        return result;
    }
}

int
MetroEvent_signal(MetroEvent e, void *value)
{
    assert(e);
    int fail = pthread_mutex_lock(&e->mutex);
    if (!fail) {
        e->state = MetroEventState_Ready;
        e->val = value;
        fail = pthread_cond_signal(&e->cond);
        if (!fail) {
            return pthread_mutex_unlock(&e->mutex);
        } else {
            return fail;
        }
    } else {
        return fail;
    }
}

int
MetroEvent_try_signal(MetroEvent e, void *value)
{
    assert(e);
    int fail = pthread_mutex_trylock(&e->mutex);
    if (!fail) {
        e->state = MetroEventState_Ready;
        e->val = value;
        fail = pthread_cond_signal(&e->cond);
        if (!fail) {
            return pthread_mutex_unlock(&e->mutex);
        } else {
            return fail;
        }
    } else {
        return fail;
    }
}

int
MetroEvent_broadcast(MetroEvent e, void *value)
{
    assert(e);
    int fail = pthread_mutex_lock(&e->mutex);
    if (!fail) {
        e->state = MetroEventState_Ready;
        e->val = value;
        fail = pthread_cond_broadcast(&e->cond);
        if (!fail) {
            return pthread_mutex_unlock(&e->mutex);
        } else {
            return fail;
        }
    } else {
        return fail;
    }
}

int
MetroEvent_try_broadcast(MetroEvent e, void *value)
{
    assert(e);
    int fail = pthread_mutex_trylock(&e->mutex);
    if (!fail) {
        e->state = MetroEventState_Ready;
        e->val = value;
        fail = pthread_cond_broadcast(&e->cond);
        if (!fail) {
            return pthread_mutex_unlock(&e->mutex);
        } else {
            return fail;
        }
    } else {
        return fail;
    }
}

void
MetroEvent_set_value(MetroEvent e, void *value)
{
    assert(e);
    e->val = value;
}

void *
MetroEvent_value(MetroEvent e)
{
    assert(e);
    return e->val;
}

void
MetroEvent_free(MetroEvent *e)
{
    assert(e && *e);
    pthread_mutex_destroy( &(*e)->mutex );
    pthread_cond_destroy( &(*e)->cond );
    FREE(*e);
}
