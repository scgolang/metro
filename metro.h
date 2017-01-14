/**
 * \brief Metronome for music applications
 *
 * All the functions in this header that expect a Metro as
 * their first argument will cause a runtime exception if
 * this Metro is NULL.
 */
#ifndef METRO_H_INCLUDED
#define METRO_H_INCLUDED

#include <stdint.h>
#include "event.h"

typedef struct Metro *Metro;

typedef float Bpm;

typedef uint64_t Nanoseconds;

/* typedef void* (*MetroFunc)(void *arg); */

/**
 * Create a metro that fires every beat according to bpm.
 * Returns NULL on error.
 */
Metro
Metro_create(Bpm bpm);

/**
 * Start a metro. Returns 0 on success, nonzero on failure.
 */
int
Metro_start(Metro metro);

/**
 * Stop a metro. Returns 0 on success, nonzero on failure.
 */
int
Metro_stop(Metro metro);

/**
 * Change tempo in bpm. Returns the old tempo.
 */
Bpm
Metro_set_bpm(Metro metro, Bpm bpm);

/**
 * Get the event used to fire off metronome ticks.
 */
MetroEvent
Metro_tick(Metro metro);

/**
 * Join to the thread running the metro.
 * This causes the calling thread to block forever.
 */
int
Metro_wait(Metro metro);

/**
 * Free the resources held by a metro.
 */
void
Metro_free(Metro *metro);

#endif
