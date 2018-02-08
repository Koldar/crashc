/*
 * stringBuilderTest.c
 *
 *  Created on: May 18, 2017
 *      Author: koldar
 */

#include <assert.h>
#include "stringBuilder.h"
#include "CuTest.h"
#include "log.h"
#include <string.h>

void testStringBuilder01(CuTest* tc) {
	StringBuilder* sb = initStringBuilderQuick();
	sb->resizeFactor = 3;
	appendToStringBuilder(sb, "hello");
	appendToStringBuilder(sb, " ");
	appendToStringBuilder(sb, "world");

	info("string is \"%s\"", getStringOfStringBuilder(sb));
	assert(strcmp(getStringOfStringBuilder(sb), "hello world") == 0);
	destroyStringBuilder(sb);
}

void testStringBuilder02(CuTest* tc) {
	StringBuilder* sb = initStringBuilderQuick();
	sb->resizeFactor = 3;
	appendToStringBuilder(sb, "hello");
	appendToStringBuilder(sb, " ");
	appendToStringBuilder(sb, "world");

	clearStringBuilder(sb);
	assert(strcmp(getStringOfStringBuilder(sb), "") == 0);
	destroyStringBuilder(sb);
}

void testStringBuilder03(CuTest* tc) {
	StringBuilder* sb = initStringBuilderQuick();
	sb->resizeFactor = 3;
	appendToStringBuilder(sb, "hello");
	appendToStringBuilder(sb, " ");
	appendToStringBuilder(sb, "world");

	clearStringBuilder(sb);
	appendToStringBuilder(sb, "bum");
	assert(strcmp(getStringOfStringBuilder(sb), "bum") == 0);
	destroyStringBuilder(sb);
}

void testStringBuilder04(CuTest* tc) {
	StringBuilder* sb = initStringBuilderQuick();
	sb->resizeFactor = 100;
	appendToStringBuilder(sb, "hello");
	appendToStringBuilder(sb, " ");
	appendToStringBuilder(sb, "world");

	assert(strcmp(getStringOfStringBuilder(sb), "hello world") == 0);
	destroyStringBuilder(sb);
}

void testStringBuilder05(CuTest* tc) {
	StringBuilder* sb = initStringBuilderQuick();
	sb->resizeFactor = 100;
	appendTemplateToStringBuilder(sb, "hello world %s!", "john");

	assert(strcmp(getStringOfStringBuilder(sb), "hello world john!") == 0);
	destroyStringBuilder(sb);
}

CuSuite* CuStringBuilderSuite() {
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, testStringBuilder01);
	SUITE_ADD_TEST(suite, testStringBuilder02);
	SUITE_ADD_TEST(suite, testStringBuilder03);
	SUITE_ADD_TEST(suite, testStringBuilder04);
	SUITE_ADD_TEST(suite, testStringBuilder05);

	return suite;
}
