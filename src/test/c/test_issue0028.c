/*
 * test_issue28.c
 *
 *  Created on: Oct 28, 2017
 *      Author: koldar
 */

#ifdef TEST_0028

#include "crashc.h"
#include "test_checker.h"

TESTS_START
REG_SUITE(1);
TESTS_END

TESTSUITE(1) {
	clear_test_checker();
	TESTCASE("test", "") {
		add_char('a');
	}
	assert_and_reset_test_checker("a");
}

#endif
