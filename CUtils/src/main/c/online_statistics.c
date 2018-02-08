/*
 * online_statistics.c
 *
 *  Created on: Aug 16, 2017
 *      Author: koldar
 */


#include "online_statistics.h"
#include <stdlib.h>
#include <math.h>
#include "macros.h"

typedef struct online_statistics {
	double average;
	double variance;
	double min;
	double max;
	long n;
	double lastValue;
};

online_statistics* initOnlineStatistics() {
	online_statistics* retVal = malloc(sizeof(online_statistics));
	if (retVal == NULL) {
		ERROR_MALLOC();
	}

	clearOnlineStatistics(retVal);

	return retVal;
}

void destroyOnlineStatistics(online_statistics* stat) {
	free(stat);
}

online_statistics* updateOnlineStatistics(online_statistics* stat, double newValue) {
	stat->lastValue = newValue;
	//number
	stat->n++;
	//average
	double delta1 = newValue - stat->average;
	stat->average += delta1/stat->n;
	//variance
	double delta2 = newValue - stat->average;
	double m2 = delta1 * delta2;
	if (stat->n < 2) {
		stat->variance = 0;
	} else {
		stat->variance = (((stat->n - 1) * stat->variance) + m2)/(stat->n);
	}
	//min
	if (newValue < stat->min) {
		stat->min = newValue;
	}
	//max
	if (newValue > stat->max) {
		stat->max = newValue;
	}

	return stat;
}

double getAverageOfOnlineStatistics(const online_statistics* stat) {
	return stat->average;
}

double getVarianceOfOnlineStatistics(const online_statistics* stat) {
	return stat->variance;
}

double getStandardDeviationOfOnlineStatistics(const online_statistics* stat) {
	return sqrt(stat->variance);
}

double getMinOfOnlineStatistics(const online_statistics* stat) {
	return stat->min;
}

double getMaxOfOnlineStatistics(const online_statistics* stat) {
	return stat->max;
}

double getMaxOrDefaultOfOnlineStatistics(const online_statistics* stat, double defaultValue) {
	return isOnlineStatisticsEmpty(stat) ? defaultValue : stat->max;
}

double getNOfOnlineStatistics(const online_statistics* stat) {
	return stat->n;
}

double getLastValueOfOnlineStatistics(const online_statistics* stat) {
	return stat->lastValue;
}

double getLastValueOrDefaultOfOnlineStatistics(const online_statistics* stat, double defaultValue) {
	return isOnlineStatisticsEmpty(stat) ? defaultValue : stat->lastValue;
}

void clearOnlineStatistics(online_statistics* stat) {
	stat->average = 0;
	stat->max = -INFINITY;
	stat->min = +INFINITY;
	stat->n = 0;
	stat->variance = 0;
	stat->lastValue = 0;
}

bool isOnlineStatisticsEmpty(const online_statistics* stat) {
	return stat->n == 0;
}
