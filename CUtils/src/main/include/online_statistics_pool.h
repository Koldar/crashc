/**
 * @file
 *
 * Represents a collection of online_statistics, nicely packed together
 *
 * This is useful because often you don't want to have just one statistic, but a batch of them. For example,
 * when testing the timings of an algorithm, it's likely you ant to test not only the total amount of time it needs, but also
 * the time of each of its subroutines, for profiling purposes.
 *
 * The APIs of this module closely relates to the ones offered for online_statistics.
 *
 * Note that, OSP acronym stands for **Online Statistics Pool**. Such acronym is used, for brevity in al lthe APIs o this module aside the
 * init and the disposer, just for clarity.
 *
 * @date Jan 4, 2018
 * @author koldar
 */

#ifndef ONLINE_STATISTICS_POOL_H_
#define ONLINE_STATISTICS_POOL_H_

#include <stdbool.h>
#include "online_statistics.h"

typedef struct online_statistics_pool online_statistics_pool;

online_statistics_pool* initOnlineStatisticsPool();

online_statistics_pool* destroyOnlineStatisticsPool(const online_statistics_pool* pool);

/**
 * Return an online_statistics in the pool with the given name
 *
 * \note
 * If the online_statistics is not present in the pool, it will be automatically be created
 *
 * @param[in] pool the pool to consider
 * @param[in] name the name of the online_statistics to get;
 * @return the associated online_statistics you wanted.
 */
online_statistics* getItemOfOSP(const online_statistics_pool* pool, const char* name);

online_statistics* updateItemInOSP(online_statistics_pool* stat, const char* name, double newValue);

/**
 * @param[in] stat the structure to analyze
 * @return
 * the average of all the values put thanks to ::updateOnlineStatistics into this structure
 */
double getAverageOfItemInOSP(const online_statistics_pool* stat, const char* name);

/**
 * @param[in] stat the structure to analyze
 * @return
 * the variance of all the values put thanks to ::updateOnlineStatistics into this structure
 */
double getVarianceOfItemInOSP(const online_statistics_pool* stat, const char* name);

/**
 * @param[in] stat the structure to analyze
 * @return
 * the standard deviation of all the values put thanks to ::updateOnlineStatistics into this structure
 */
double getStandardDeviationOfItemInOSP(const online_statistics_pool* stat, const char* name);

/**
 * @param[in] stat the structure to analyze
 * @return
 * the minimum of all the values put thanks to ::updateOnlineStatistics into this structure
 */
double getMinOfItemInOSP(const online_statistics_pool* stat, const char* name);

/**
 * @param[in] stat the structure to analyze
 * @return
 * the maximum of all the values put thanks to ::updateOnlineStatistics into this structure
 */
double getMaxOfItemInOSP(const online_statistics_pool* stat, const char* name);

/**
 * like ::getMaxOfOnlineStatistics but return a default value if the online_statistics is empty
 *
 * @param[in] stat the statistics to poll
 * @param[in] defaultValue the value to return if \c stat is empty
 * @return the desired value
 */
double getMaxOrDefaultOfItemInOSP(const online_statistics_pool* stat, const char* name, double defaultValue);

/**
 * @param[in] stat the structure to analyze
 * @return
 * the number of values put thanks to ::updateOnlineStatistics into this structure
 */
double getNOfItemInOSP(const online_statistics_pool* stat, const char* name);

/**
 * @param[in] stat the structure to analyze
 * @return
 * the last value you have put with ::updateOnlineStatistics into this structure
 */
double getLastValueOfItemInOSP(const online_statistics_pool* stat, const char* name);

/**
 * like ::getLastValueOfOnlineStatistics but return a default value if the online_statistics is empty
 *
 * @param[in] stat the statistics to poll
 * @param[in] defaultValue the value to return if \c stat is empty
 * @return the desired value
 */
double getLastValueOrDefaultOfItemInOSP(const online_statistics_pool* stat, const char* name, double defaultValue);

/**
 * Reset the statistics as if no data ever arrived
 *
 * \post
 * 	\li after this operation ::getNOfOnlineStatistics will return 0
 *
 * @param[inout] stat the stat to update
 */
void clearItemInOSP(online_statistics* stat, const char* name);

/**
 *
 * @param[inout] stat the statistics to poll
 * @return
 * 	\li true if we didn't add even 1 data inside this statistics;
 * 	\li false otherwise
 */
bool isItemInOSPEmpty(const online_statistics* stat, const char* name);

#endif /* ONLINE_STATISTICS_POOL_H_ */
