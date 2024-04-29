#pragma once

#include <assert.h>
#include <fcntl.h>
#include <time.h>
#include <math.h>
#include <pthread.h>

#include "cbos.h"

/**
 * Data structure for tracking progress during a task
 */
typedef struct Progress
{
    double elapsed_time;
    size_t iterations_completed;
    size_t iterations_total;
    const char *lib_name;
    double start_time;
} Progress;

/**
 * @brief Monitors task progress in a separate thread.
 *
 * @param arg Pointer to a Progress structure.
 * @return NULL.
 */
void *progress_function(void *arg);

/**
 * @brief Function to get the current timestamp.
 *
 * @return The current timestamp.
 */
unsigned int timestamp(void);

/**
 * @brief Function to initialize the Crypto Counter (ccnt).
 */
void ccnt_init(void);

/**
 * @brief Returns the current time in seconds.
 *
 * @return Current time in seconds.
 */
double seconds(void);