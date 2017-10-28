/*
 * test_issue28.c
 *
 *  Created on: Oct 28, 2017
 *      Author: koldar
 */

#ifdef TEST_0028

#include "crashC.all.in.one.h"
#include "test_checker.h"

TESTS_START
REG_SUITE(1);
TESTS_END

TESTSUITE(1) {
	clearTestChecker();
	TESTCASE("test") {
		addCharacter('a');
	}
	assertTestCheckerAndReset('a');
}

#endif
