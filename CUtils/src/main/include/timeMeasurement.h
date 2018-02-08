/*
 * timeMeasurement.h
 *
 *  Created on: Jan 19, 2017
 *      Author: koldar
 */

#ifndef TIMEMEASUREMENT_H_
#define TIMEMEASUREMENT_H_

#include <time.h>
#include <math.h>
#include <limits.h>
#include <stdbool.h>

#define MAX_PROFILE_ALLOWED 5

typedef enum TimeMeasurement {
	TM_NANO = 0,
	TM_MICRO = 1,
	TM_MILLI = 2,
	TM_SECONDS = 3
} TimeMeasurement;

typedef struct PerformanceIndicator {
	struct timespec start;
	struct timespec end;

	unsigned long t;
	double avg;

	double delta1;
	double delta2;
	double m2;
	double sd;
	unsigned long n;
	int i;
} PerformanceIndicator;
extern PerformanceIndicator performanceIndicators[MAX_PROFILE_ALLOWED];
extern bool clearPerformanceIndicators;

/**
 * Perform a timing of a piece of code
 *
 * You can use this macro like this:
 *
 * ```
 * TIME_PROFILE_CODE(delta, TM_NANO) {
 * 		printf("Hello world!\n");
 * }
 * printf("time required to compute printf in nanoseconds=%ld\n", delta);
 * ```
 *
 * @param[in] timegap the name of the variable that after the code has been execute will contain the number of seconds/milliseconds/microseconds/nanoseconds
 * 		tha tthe code took to be executed. The variable last after the macro call; You don't need to create it
 * @param[in] unit_measure a value of type ::TimeMeasurement. this will be the unit measure of \c timegap
 */
#define TIME_PROFILE_CODE(timegap, unit_measure) \
	long timegap; \
	TimeMeasurement UV(um) = unit_measure; \
	struct timespec UV(start); \
	struct timespec UV(end); \
	UV(start) = getCurrentTime(); \
	for ( \
			bool UV(condition)=true \
			; \
			UV(condition) \
			; \
			UV(end) = getCurrentTime(),\
			UV(condition)=false, \
			timegap = computeTimeGap(UV(start), UV(end), UV(um))\
	)


//TODO doc
#define PROFILECODEWITHDESC(id, description) \
		if (clearPerformanceIndicators) { \
			memset(performanceIndicators, 0, sizeof(PerformanceIndicator) * MAX_PROFILE_ALLOWED); \
			clearPerformanceIndicators = false; \
		} \
		for ( \
				performanceIndicators[id].i = 0, \
				clock_gettime(CLOCK_MONOTONIC, &performanceIndicators[id].start) \
				; \
				performanceIndicators[id].i < 1 \
				; \
				clock_gettime(CLOCK_MONOTONIC, &performanceIndicators[id].end), \
				performanceIndicators[id].t = computeTimeGap(performanceIndicators[id].start, performanceIndicators[id].end, TM_MICRO), \
				performanceIndicators[id].n++, \
				performanceIndicators[id].delta1 = performanceIndicators[id].t - performanceIndicators[id].avg, \
				performanceIndicators[id].avg += (performanceIndicators[id].delta1 + 0.)/performanceIndicators[id].n, \
				performanceIndicators[id].delta2 = performanceIndicators[id].t - performanceIndicators[id].avg, \
				performanceIndicators[id].m2 += performanceIndicators[id].delta1 * performanceIndicators[id].delta2, \
				performanceIndicators[id].sd = sqrt(performanceIndicators[id].n >= 2 ? ((performanceIndicators[id].m2 + 0.)/(performanceIndicators[id].n - 1)): 0), \
				performanceIndicators[id].i++, \
				critical("%s took %ld (n=%ld avg=%.2f sd=%.2f)", description, performanceIndicators[id].t, performanceIndicators[id].n, performanceIndicators[id].avg, performanceIndicators[id].sd) \
		)

#define PROFILECODE(id) PROFILECODEWITHDESC(id, __func__)

/**
 * @param[in] tm the measurement involved
 * @return a string representation of \c tm
 */
const char* getStringOfTimeMeasurement(TimeMeasurement tm);

/**
 * @param[in] a string representing a time measurement. Allowed values are "ns", "us", "ms" or "s"
 * @return the time measurement from a given string
 */
TimeMeasurement parseTimeMeasurementFromString(const char* s);

struct timespec getCurrentTime();

long computeTimeGap(struct timespec start, struct timespec end, TimeMeasurement format);

#endif /* TIMEMEASUREMENT_H_ */
