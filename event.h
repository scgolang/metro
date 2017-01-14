/**
 * MetroEvents are used for inter-thread synchronization
 * MetroEvent_wait and MetroEvent_timedwait will block the calling
 * thread until another thread calls MetroEvent_signal or
 * MetroEvent_broadcast
 */
#ifndef EVENT_H_INCLUDED
#define EVENT_H_INCLUDED

typedef struct MetroEvent *MetroEvent;

MetroEvent
MetroEvent_init();

/**
 * Lock the event's mutex
 */
int
MetroEvent_lock(MetroEvent e);

int
MetroEvent_wait(MetroEvent e);

int
MetroEvent_timedwait(MetroEvent e, long ns);

/**
 * Signal the event.
 * This function can block for a long time because
 * it uses Mutex_lock to acquire the MetroEvent's mutex
 */
int
MetroEvent_signal(MetroEvent e, void *value);

/**
 * Signal the event, but use Mutex_trylock
 * to acquire the lock.
 */
int
MetroEvent_try_signal(MetroEvent e, void *value);

/**
 * Broadcast the event.
 * This function can block for a long time because
 * it uses Mutex_lock to acquire the MetroEvent's mutex
 */
int
MetroEvent_broadcast(MetroEvent e, void *value);

/**
 * Broadcast the event, but use Mutex_trylock
 * to acquire the lock.
 */
int
MetroEvent_try_broadcast(MetroEvent e, void *value);

void
MetroEvent_set_value(MetroEvent e, void *value);

void *
MetroEvent_value(MetroEvent e);

void
MetroEvent_free(MetroEvent *e);

#endif
