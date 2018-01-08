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
	ct_add_tail_in_list((ct_model)->current_snapshot->assertion_reports, ct_init_assert_report(1, "sum >= 0 && sum <= 10", "/home/koldar/git/crashc/src/test/c/test_issue0069.c", 40));		\
	if ((sum >= 0 && sum <= 10) != 1) {																												\
		ct_general_assert_failed((ct_model));																											\
	}																																		\
	else {																																	\
		ct_assert_do_nothing((ct_model));																											\
	}
	//ASSERT(sum >= 0 && sum <= 10);
}

TESTSUITE(5) {
	sum += 1;
	ASSERT(sum >= 0 && sum <= 10);
}

TESTSUITE(6) {
	sum += 1;
	ASSERT(sum >= 0 && sum <= 10);
}

TESTSUITE(7) {
	sum += 1;
	ASSERT(sum >= 0 && sum <= 10);
}

TESTSUITE(8) {
	sum += 1;
	ASSERT(sum >= 0 && sum <= 10);
}

TESTSUITE(10) {
	sum += 1;
	ASSERT(sum >= 0 && sum <= 10);
}

TESTSUITE(11) {
	sum += 1;
	ASSERT(sum >= 0 && sum <= 10);
}

TESTSUITE(hello) {
	sum += 1;
	ASSERT(sum >= 0 && sum <= 10);
}

TESTSUITE(world) {
	sum += 1;
	ASSERT(sum >= 0 && sum <= 10);
}

TESTSUITE(glhf) {
	sum += 1;
	ASSERT(sum >= 0 && sum <= 10);
}

#endif
