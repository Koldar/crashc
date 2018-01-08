/**
 * @file
 *
 * @author koldar
 * @date Jan 8, 2018
 */

#ifdef TEST_0071

#include <stdio.h>
#include "crashC.h"

TESTS_START
REG_SUITE(0);
TESTS_END

TESTSUITE(0) {
	int sum = 0;
	sum += 1;
	ASSERT(sum >= 0 && sum <= 10);
}

#endif
