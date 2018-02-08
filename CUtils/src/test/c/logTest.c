/*
 * logTest.c
 *
 *  Created on: Jun 23, 2017
 *      Author: koldar
 */


#include "log.h"
#include <assert.h>
#include "CuTest.h"
#include <stdio.h>

void testLog01(CuTest* tc) {
	debug("hello");
	debug("hello %s", "world");

	int i = 0;
	gdebug(i++, "increasing i");
#if QUICK_LOG > __LOG_DEBUG
	assert(i == 0);
#else
	assert(i == 1);
#endif
	char name[] = "pippo";
	bdebug("print name: %s", (sprintf(log_buffer, "%s", name), log_buffer));

	sbdebug(200, "print name again: %s", (sprintf(log_buffer, "%s", name), log_buffer));
}

void testLog02(CuTest* tc) {
	critical("hello");
	critical("hello %s", "world");

	int i = 0;
	gcritical(i++, "increasing i");
#if QUICK_LOG > __LOG_CRITICAL
	assert(i == 0);
#else
	assert(i == 1);
#endif
	char name[] = "pippo";
	bcritical("print name: %s", (sprintf(log_buffer, "%s", name), log_buffer));

	sbcritical(200, "print name again: %s", (sprintf(log_buffer, "%s", name), log_buffer));
}

void testLog03(CuTest* tc) {
	info("looking for >= or > constraints to convert in <= or <");
}

CuSuite* CuLogSuite() {
	CuSuite* suite = CuSuiteNew();


	SUITE_ADD_TEST(suite, testLog01);
	SUITE_ADD_TEST(suite, testLog02);
	SUITE_ADD_TEST(suite, testLog03);

	return suite;
}
