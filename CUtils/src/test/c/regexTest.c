/*
 * regexTest.c
 *
 *  Created on: May 18, 2017
 *      Author: koldar
 */

#include <assert.h>
#include "regularExpression.h"
#include "CuTest.h"
#include <stdbool.h>
#include <string.h>
#include "log.h"

///Test simple regex
void testRegex01(CuTest* tc) {
	char* string = "Hello wo123rld!";
	char** groups = NULL;

	bool r = applyRegularExpression(string, "o[0-9][0-9]*r", 0, NULL);
	assert(r == true);


}

void testRegex02(CuTest* tc) {
	char* string = "Hello wo123rld!";
	char** groups = NULL;

	bool r = applyRegularExpression(string, "o[[:digit:]][[:digit:]]*r", 0, NULL);
	assert(r == true);
}

void testRegex03(CuTest* tc) {
	char* string = "Hello wo123rld!";
	char** groups = NULL;

	bool r = applyRegularExpression(string, "o\\([[:digit:]][[:digit:]]*\\)r", 1, &groups);
	assert(r == true);

	info("group 0 \"%s\"", (groups[0]));
	info("group 1 \"%s\"", (groups[1]));
	assert(strcmp(groups[0], "o123r") == 0);
	assert(strcmp(groups[1], "123") == 0);

	destroyGroupInfo(1, &groups);
}

//doesn't match
void testRegex04(CuTest* tc) {
	char* string = "Hello world!";
	char** groups = NULL;

	bool r = applyRegularExpression(string, "o[[:digit:]][[:digit:]]*r", 0, NULL);
	assert(r == false);
}

//regex syntax error
void testRegex05(CuTest* tc) {
	char* string = "Hello wo123rld!";
	char** groups = NULL;

	bool r = applyRegularExpression(string, "o[:digit:][:digit:]*r", 0, NULL);
	assert(r == false);
}

CuSuite* CuRegexSuite() {
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, testRegex01);
	SUITE_ADD_TEST(suite, testRegex02);
	SUITE_ADD_TEST(suite, testRegex03);
	SUITE_ADD_TEST(suite, testRegex04);
	SUITE_ADD_TEST(suite, testRegex05);

	return suite;
}

