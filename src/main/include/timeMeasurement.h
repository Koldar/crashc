#ifndef TIMEMEASUREMENT_H_
#define TIMEMEASUREMENT_H_

#include <time.h>

struct timespec getCurrentTime();
long computetimeGap(struct timespec start, struct timespec end, TimeFormat format);


#endif
