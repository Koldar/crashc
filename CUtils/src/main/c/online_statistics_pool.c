/*
 * online_statistics_pool.c
 *
 *  Created on: Jan 4, 2018
 *      Author: koldar
 */

#include "online_statistics_pool.h"
#include "defaultFunctions.h"
#include <stdlib.h>
#include "hashtable.h"
#include "macros.h"

struct online_statistics_pool {
	/**
	 * Hashtable keyed with strings and online_statistics as values
	 *
	 * keys are assumed to be in the data sector of the application while values are assumed to be in the heap
	 */
	HT* statistics;
};

online_statistics_pool* initOnlineStatisticsPool() {
	online_statistics_pool* retVal = malloc(sizeof(online_statistics_pool));
	if (retVal == NULL) {
		ERROR_MALLOC();
	}

	retVal->statistics = initHT();

	return retVal;
}

online_statistics_pool* destroyOnlineStatisticsPool(const online_statistics_pool* pool) {
	CORRECT_ITERATE_VALUES_ON_HT(&pool->statistics, stat, online_statistics*) {
		destroyOnlineStatistics(stat);
	}
	free(pool);
}

online_statistics* getItemOfOSP(const online_statistics_pool* pool, const char* name) {
	online_statistics* retVal = getItemInHT(&pool->statistics, name);
	if (retVal == NULL) {
		retVal = initOnlineStatistics();
		addItemInHTWithKey(&pool->statistics, hashString(name), retVal);
	}
	return retVal;
}

online_statistics* updateItemInOSP(online_statistics_pool* stat, const char* name, double newValue) {
	online_statistics* retVal = getItemOfOSP(stat, name);
	return updateOnlineStatistics(retVal, newValue);
}

double getAverageOfItemInOSP(const online_statistics_pool* stat, const char* name) {
	online_statistics* retVal = getItemOfOSP(stat, name);
	return getAverageOfOnlineStatistics(retVal);
}

double getVarianceOfItemInOSP(const online_statistics_pool* stat, const char* name) {
	online_statistics* retVal = getItemOfOSP(stat, name);
	return getVarianceOfOnlineStatistics(retVal);
}

double getStandardDeviationOfItemInOSP(const online_statistics_pool* stat, const char* name) {
	online_statistics* retVal = getItemOfOSP(stat, name);
	return getStandardDeviationOfOnlineStatistics(retVal);
}

double getMinOfItemInOSP(const online_statistics_pool* stat, const char* name) {
	online_statistics* retVal = getItemOfOSP(stat, name);
	return getMinOfOnlineStatistics(retVal);
}

double getMaxOfItemInOSP(const online_statistics_pool* stat, const char* name) {
	online_statistics* retVal = getItemOfOSP(stat, name);
	return getMaxOfOnlineStatistics(retVal);
}

double getMaxOrDefaultOfItemInOSP(const online_statistics_pool* stat, const char* name, double defaultValue) {
	online_statistics* retVal = getItemOfOSP(stat, name);
	return getMaxOrDefaultOfOnlineStatistics(retVal, defaultValue);
}

double getNOfItemInOSP(const online_statistics_pool* stat, const char* name) {
	online_statistics* retVal = getItemOfOSP(stat, name);
	return getNOfOnlineStatistics(retVal);
}

double getLastValueOfItemInOSP(const online_statistics_pool* stat, const char* name) {
	online_statistics* retVal = getItemOfOSP(stat, name);
	return getLastValueOfOnlineStatistics(retVal);
}

double getLastValueOrDefaultOfItemInOSP(const online_statistics_pool* stat, const char* name, double defaultValue) {
	online_statistics* retVal = getItemOfOSP(stat, name);
	return getLastValueOrDefaultOfOnlineStatistics(retVal, defaultValue);
}

void clearItemInOSP(online_statistics* stat, const char* name) {
	online_statistics* retVal = getItemOfOSP(stat, name);
	clearOnlineStatistics(retVal);
}

bool isItemInOSPEmpty(const online_statistics* stat, const char* name) {
	online_statistics* retVal = getItemOfOSP(stat, name);
	return isOnlineStatisticsEmpty(retVal);
}
