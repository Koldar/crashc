/**
 * @file
 *
 * Contains API to track elapsed running time of a process
 *
 * @author noodles
 * @date Oct 13, 2017
 */

#ifndef TIMEMEASUREMENT_H_
#define TIMEMEASUREMENT_H_

#include <time.h>

/**
 * Get the time *now*
 *
 * @return struct holding information about the time when this function has been called
 */
struct timespec getCurrentTime();
/**
 * Get a time interval
 *
 * @param[in] start the start of the time interval
 * @param[in] end the end of the time interval
 * @param[in] format_str either one of the following options:
 * 	\li "s" if you want to the time interval in seconds;
 *  \li "m" if you want to the time interval in milliseconds;
 *  \li "u" if you want to the time interval in microseconds;
 *  \li "n" if you want to the time interval in nanoseconds;
 * @return the time elapsed between \c start and \c end
 * @see getCurrentTime
 */
long computetimeGap(struct timespec start, struct timespec end, const char * format_str);


#endif
