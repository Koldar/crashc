/*
 * random_utils.h
 *
 *  Created on: Jul 18, 2017
 *      Author: koldar
 */

#include "random_utils.h"
#include <stdlib.h>
#include <limits.h>
#include "macros.h"
#include "log.h"

static int rand_lim(int limit);

/* return a random number between 0 and limit inclusive.
 *
 * <a href="https://stackoverflow.com/a/2999130/1887602">Stack Overflow answer</a>
 */
static int rand_lim(int limit) {
	int divisor;

	if (limit < RAND_MAX) {
		divisor = RAND_MAX/(limit+1);
	} else if (limit == RAND_MAX) {
		divisor = 1;
	} else {
		ERROR_APPLICATION_FAILED("random", "rand_lim", "number", limit);
	}
    int retval;

    do {
        retval = random() / divisor;
    } while (retval > limit);

    return retval;
}

int get_random(int min, int max, bool minIncluded, bool maxIncluded) {
	min += minIncluded ? 0 : 1;
	max -= maxIncluded ? 0 : 1;

	return min + rand_lim(max - min);
}


int getRandomInRange(int min, int max, bool minIncluded, bool maxIncluded) {
	min += minIncluded ? 0 : 1;
	max += maxIncluded ? 1 : 0;

	//now min is always included and max is always excluded
	return min + rand_lim(max - min - 1);
	//return min + (rand() % (max - min));
}

int getRandomValueInArray(int n, const int array[]) {
	return array[getRandomInRange(0, n, true, false)];
}

bool getRandomInProbability(double probability) {
	int p = (int)(probability * ((double)RAND_MAX));
	return getRandomInRange(0, RAND_MAX, true, false) < p;
}
