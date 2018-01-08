/**
 * @file
 *
 * @author koldar
 * @date Jan 8, 2018
 */

#ifdef TEST_0069

#include <stdio.h>
#include "crashC.h"

static int sum = 0;

void globalAssert() {
	if (sum == 10) {
		//this is the only print. Tests with no print are considered to be failed
		printf("OK!\n");
	} else {
		printf("sum is %d\n",sum);
		exit(1);
	}
}

TESTS_START
//test several registration
REGTESTS(3,5,6,7,8,10);
//test a single registration
REGTESTS(11);

//test several registration with identifier
REGTESTS(hello, world);
//test a single registration with identifier
REGTESTS(glhf);
AFTER_TESTS(globalAssert);
TESTS_END

TESTSUITE(3) {
	sum += 1;
}

TESTSUITE(5) {
	sum += 1;
}

TESTSUITE(6) {
	sum += 1;
}

TESTSUITE(7) {
	sum += 1;
}

TESTSUITE(8) {
	sum += 1;
}

TESTSUITE(10) {
	sum += 1;
}

TESTSUITE(11) {
	sum += 1;
}

TESTSUITE(hello) {
	sum += 1;
}

TESTSUITE(world) {
	sum += 1;
}

TESTSUITE(glhf) {
	sum += 1;
}

#endif
