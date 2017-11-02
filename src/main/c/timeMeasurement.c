#include "timeMeasurement.h"

typedef enum {
	TF_NANO    = 0,
	TF_MICRO   = 1,
	TF_MILLI   = 2,
	TF_SECONDS = 3
} TimeFormat;

static TimeFormat parseTimeFormatFromString(const char * str) {
	switch (*str) {
	case 's': return TF_SECONDS;
	case 'm': return TF_MILLI;
	case 'u': return TF_MICRO;
	case 'n': return TF_NANO;
	}
}

struct timespec getCurrentTime() {
	struct timespec retVal;
	clock_gettime(CLOCK_MONOTONIC, &retVal);
	return retVal;
}

long computetimeGap(struct timespec start, struct timespec end, const char * format_str)  {
	long sec, nanoSec, retVal;
	TimeFormat format = parseTimeFormatFromString(format_str);

	sec = end.tv_sec - start.tv_sec;
	nanoSec = end.tv_nsec - start.tv_nsec;

	retVal = ((sec * 1E9 + nanoSec) + nanoSec);
	for (int i = 0; i < format; i++) {
		retVal /= 1000;
	}

	return retVal;
}
