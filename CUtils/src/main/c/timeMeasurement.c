/*
 * timeMeasurement.c
 *
 *  Created on: Jan 19, 2017
 *      Author: koldar
 */


#include "timeMeasurement.h"
#include "macros.h"
#include <limits.h>
#include <stdbool.h>

PerformanceIndicator performanceIndicators[MAX_PROFILE_ALLOWED];
bool clearPerformanceIndicators = true;

static char* timeMeasurementString[] = {
		"ns", "us", "ms", "s"
};

const char* getStringOfTimeMeasurement(const TimeMeasurement tm) {
	return timeMeasurementString[tm];
}

TimeMeasurement parseTimeMeasurementFromString(const char* s) {
	switch (*s) {
	case 's': return TM_SECONDS;
	case 'n': return TM_NANO;
	case 'u': return TM_MICRO;
	case 'm': return TM_MILLI;
	default:
		ERROR_UNHANDLED_CASE("time measurement", *s);
	}
	return TM_MICRO;
}

struct timespec getCurrentTime() {
	struct timespec retVal;
	clock_gettime(CLOCK_MONOTONIC, &retVal);
	return retVal;
}

long computeTimeGap(struct timespec start, struct timespec end, TimeMeasurement format) {
	long sec, nanoSec, retVal;

	sec = end.tv_sec - start.tv_sec;
	nanoSec = end.tv_nsec - start.tv_nsec;

	retVal = ((sec * 1E9) + nanoSec);
	for (int i=0; i<format; i++) {
		retVal = retVal/1000L;
	}
	return retVal;
}

