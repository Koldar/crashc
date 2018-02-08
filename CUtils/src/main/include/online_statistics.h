/*
 * \file online_statistics.h
 *
 * Contains some structures useful to check statistical numbers **online**
 *
 * By **online** we mean that the statistical numbers will be updated while the algorithm under
 * analysis is running.
 *
 * For example the 2 statistical number to check might be average and variance
 *
 * You can use this module like this:
 *
 * ```
 * online_statistics* tmp = initOnlineStatistics();
 * updateOnlineStatistics(tmp, 1);
 * updateOnlineStatistics(tmp, 3);
 * updateOnlineStatistics(tmp, 5);
 * assert(getAverageOfOnlineStatistics(tmp) == 3);
 * destroyOnlineStatistics(tmp);
 *
 *
 *  Created on: Aug 16, 2017
 *      Author: koldar
 */

#ifndef ONLINE_STATISTICS_H_
#define ONLINE_STATISTICS_H_

#include <stdbool.h>

typedef struct online_statistics online_statistics;

/**
 * Generates a new ::online_statistics structure
 *
 * \post
 * 	\li new bytes have been allocated in the heap
 *
 * @return
 * 	\li a newly initialized structure
 */
online_statistics* initOnlineStatistics();

/**
 * Release from memory a given ::online_statistics
 *
 * \note
 * Use it to free the bytes allocated from ::initOnlineStatistics
 *
 * @param[in] stat the structure to dispose of
 */
void destroyOnlineStatistics(online_statistics* stat);

/**
 * Adds a new value inside the statistics and update the statistics themselves
 *
 * \post
 * 	\li the statistics (aka mean, variance and so on) may have changed
 *
 * @param[inout] stat the statistics to update
 * @param[in] newValue the new value inside the stream of data observed by \c stat
 * @return \c stat itself
 */
online_statistics* updateOnlineStatistics(online_statistics* stat, double newValue);

/**
 * @param[in] stat the structure to analyze
 * @return
 * the average of all the values put thanks to ::updateOnlineStatistics into this structure
 */
double getAverageOfOnlineStatistics(const online_statistics* stat);

/**
 * @param[in] stat the structure to analyze
 * @return
 * the variance of all the values put thanks to ::updateOnlineStatistics into this structure
 */
double getVarianceOfOnlineStatistics(const online_statistics* stat);

/**
 * @param[in] stat the structure to analyze
 * @return
 * the standard deviation of all the values put thanks to ::updateOnlineStatistics into this structure
 */
double getStandardDeviationOfOnlineStatistics(const online_statistics* stat);

/**
 * @param[in] stat the structure to analyze
 * @return
 * the minimum of all the values put thanks to ::updateOnlineStatistics into this structure
 */
double getMinOfOnlineStatistics(const online_statistics* stat);

/**
 * @param[in] stat the structure to analyze
 * @return
 * the maximum of all the values put thanks to ::updateOnlineStatistics into this structure
 */
double getMaxOfOnlineStatistics(const online_statistics* stat);

/**
 * like ::getMaxOfOnlineStatistics but return a default value if the online_statistics is empty
 *
 * @param[in] stat the statistics to poll
 * @param[in] defaultValue the value to return if \c stat is empty
 * @return the desired value
 */
double getMaxOrDefaultOfOnlineStatistics(const online_statistics* stat, double defaultValue);

/**
 * @param[in] stat the structure to analyze
 * @return
 * the number of values put thanks to ::updateOnlineStatistics into this structure
 */
double getNOfOnlineStatistics(const online_statistics* stat);

/**
 * @param[in] stat the structure to analyze
 * @return
 * the last value you have put with ::updateOnlineStatistics into this structure
 */
double getLastValueOfOnlineStatistics(const online_statistics* stat);

/**
 * like ::getLastValueOfOnlineStatistics but return a default value if the online_statistics is empty
 *
 * @param[in] stat the statistics to poll
 * @param[in] defaultValue the value to return if \c stat is empty
 * @return the desired value
 */
double getLastValueOrDefaultOfOnlineStatistics(const online_statistics* stat, double defaultValue);

/**
 * Reset the statistics as if no data ever arrived
 *
 * \post
 * 	\li after this operation ::getNOfOnlineStatistics will return 0
 *
 * @param[inout] stat the stat to update
 */
void clearOnlineStatistics(online_statistics* stat);

/**
 *
 * @param[inout] stat the statistics to poll
 * @return
 * 	\li true if we didn't add even 1 data inside this statistics;
 * 	\li false otherwise
 */
bool isOnlineStatisticsEmpty(const online_statistics* stat);

#endif /* ONLINE_STATISTICS_H_ */
