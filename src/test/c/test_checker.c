/*
 * \file test_checker.c
 *
 * A naive module us
 *
 *  Created on: Sep 28, 2017
 *      Author: koldar
 */

#include "test_checker.h"
#include <stdio.h>
#include <assert.h>

static char __TEST_CHECKER_BUFFER[1000];
static int nextPosition;
static char split = ' ';

void clearTestChecker() {
	nextPosition = 0;
	__TEST_CHECKER_BUFFER[nextPosition] = '\0';
}

void setSplit(char s) {
	split = s;
}

void addCharacter(char ch) {
	__TEST_CHECKER_BUFFER[nextPosition] = ch;
	nextPosition += 1;
	__TEST_CHECKER_BUFFER[nextPosition] = '\0';
}

void addString(const char* str) {
	strcpy(&__TEST_CHECKER_BUFFER[nextPosition], str);
	nextPosition += strlen(str);
	addCharacter(split);
}

bool checkTestChecker(const char* expected) {
	return (strcmp(__TEST_CHECKER_BUFFER, expected) == 0);
}

bool checkTestCheckerAndReset(const char* expected) {
	bool retVal = (strcmp(__TEST_CHECKER_BUFFER, expected) == 0);
	clearTestChecker();
	return retVal;
}

void assertTestCheckerAndReset(const char* expected) {
	if (!checkTestChecker(expected)) {
		fprintf(stdout, "KO!\n");
		fprintf(stderr, "EXPECTED: %s\nACTUAL:   %s\n", expected, __TEST_CHECKER_BUFFER);
		assert(false);
	} else {
		fprintf(stdout, "OK!\n");
	}
	clearTestChecker();
}
