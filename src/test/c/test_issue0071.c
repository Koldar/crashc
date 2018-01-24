/**
 * @file
 *
 * @author koldar
 * @date Jan 8, 2018
 */

#ifdef TEST_0071

#include <stdio.h>
#include "crashc.h"

TESTS_START
REG_SUITE(0);
TESTS_END

TESTSUITE(0) {
	//This testcase is here only to prevent the test to fail, as the issue is marked as wontfix
	// due to it being related to an unsupported feature of CrashC in the version in which the issue
	// was created.
	TESTCASE("testcase", "") {
		int sum = 0;
		sum += 1;
		ASSERT(sum >= 0 && sum <= 10);
	}
}

#endif
