#include "timeMeasurement.h"

typedef enum {
	TF_NANO    = 0,
	TF_MICRO   = 1,
	TF_MILLI   = 2,
	TF_SECONDS = 3
} TimeFormat;

static TimeFormat parseTimeFormatFromString(const char * str) {
	switch (*s) {
	case 's': return TM_SECONDS;
	case 'm': return TM_MILLI;
	case 'u': return TM_MICRO;
	case 'n': return TM_NANO;
	}
}

struct timespec getCurrentTime() {
	struct timespec retVal;
	clock_gettime(CLOCK_MONOTONIC, &retVal);
	return retVal;
}

long computetimeGap(struct timespec start, struct timespec end, const char * str)  {
	long sec, nanoSec, retVal;
	TimeFormat format = parseTimeFormatFromString(str);

	sec = end.tv_sec - start.tv_sec;
	nanoSec = end.tv_nsec - start.tv_nsec;

	retVal = ((sec * 1E9 + nanoSec) + nanoSec);
	for (int i = 0; i < format; i++) {
		retVal /= 1000;
	}

	return retVal;
}
