/*
 * dynamic_stackTest.c
 *
 *  Created on: Oct 11, 2017
 *      Author: koldar
 */

#include "CuTest.h"
#include "dynamic_stack.h"
#include <assert.h>

void test01(CuTest* tc) {
	dynamic_stack* s = initDynamicStack(5, 3, getIntPayloadFunctions());

	assert(isDynamicStackEmpty(s));
	assert(!isDynamicStackFull(s));
	assert(getSizeOfDynamicStack(s) == 0);

	destroyDynamicStackWithElements(s);
}

void test02(CuTest* tc) {
	dynamic_stack* s = initDynamicStack(5, 3, getIntPayloadFunctions());

	pushItemToDynamicStack(s, 5);
	assert(!isDynamicStackEmpty(s));
	assert(!isDynamicStackFull(s));
	assert(getSizeOfDynamicStack(s) == 1);
	pushItemToDynamicStack(s, 3);
	pushItemToDynamicStack(s, 2);
	pushItemToDynamicStack(s, 4);
	pushItemToDynamicStack(s, 7);
	assert(!isDynamicStackEmpty(s));
	assert(isDynamicStackFull(s));
	assert(getSizeOfDynamicStack(s) == 5);

	destroyDynamicStackWithElements(s);
}

void test03(CuTest* tc) {
	dynamic_stack* s = initDynamicStack(5, 3, getIntPayloadFunctions());

	pushItemToDynamicStack(s, 5);
	assert(!isDynamicStackEmpty(s));
	assert(!isDynamicStackFull(s));
	assert(getSizeOfDynamicStack(s) == 1);
	pushItemToDynamicStack(s, 3);
	pushItemToDynamicStack(s, 2);
	pushItemToDynamicStack(s, 4);
	pushItemToDynamicStack(s, 7);
	assert(!isDynamicStackEmpty(s));
	assert(isDynamicStackFull(s));
	assert(getSizeOfDynamicStack(s) == 5);

	pushItemToDynamicStack(s, 8);
	assert(!isDynamicStackEmpty(s));
	assert(!isDynamicStackFull(s));
	assert(getSizeOfDynamicStack(s) == 6);

	destroyDynamicStackWithElements(s);
}

void test04(CuTest* tc) {
	dynamic_stack* s = initDynamicStack(5, 3, getIntPayloadFunctions());

	pushItemToDynamicStack(s, 5);
	assert(!isDynamicStackEmpty(s));
	assert(!isDynamicStackFull(s));
	assert(getSizeOfDynamicStack(s) == 1);
	pushItemToDynamicStack(s, 3);
	pushItemToDynamicStack(s, 2);
	pushItemToDynamicStack(s, 4);
	pushItemToDynamicStack(s, 7);
	assert(!isDynamicStackEmpty(s));
	assert(isDynamicStackFull(s));
	assert(getSizeOfDynamicStack(s) == 5);

	pushItemToDynamicStack(s, 8);
	assert(!isDynamicStackEmpty(s));
	assert(!isDynamicStackFull(s));
	assert(getSizeOfDynamicStack(s) == 6);

	assert(peekItemFromDynamicStack(s) == 8);
	assert(popItemFromDynamicStack(s) == 8);
	assert(!isDynamicStackEmpty(s));
	assert(!isDynamicStackFull(s));
	assert(getSizeOfDynamicStack(s) == 5);

	destroyDynamicStackWithElements(s);
}

CuSuite* CuDynamicStackSuite() {
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, test01);
	SUITE_ADD_TEST(suite, test02);
	SUITE_ADD_TEST(suite, test03);
	SUITE_ADD_TEST(suite, test04);

	return suite;
}
