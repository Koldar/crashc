/*
 * stackTest.c
 *
 *  Created on: May 18, 2017
 *      Author: koldar
 */

#include <assert.h>
#include "stack.h"
#include "CuTest.h"
#include <stdbool.h>
#include "log.h"

///Test push, size of stack
void testStack01(CuTest* tc) {
	Stack* stack = initStack(3);

	int values[] = {1,2,3,4,5,6,7,8,9,10};

	assert(isStackEmpty(stack) == true);
	assert(isStackFull(stack) == false);
	assert(getStackSize(stack) == 0);

	pushToStack(stack, &values[0]);

	assert(isStackEmpty(stack) == false);
	assert(isStackFull(stack) == false);
	assert(getStackSize(stack) == 1);

	pushToStack(stack, &values[1]);

	assert(isStackEmpty(stack) == false);
	assert(isStackFull(stack) == false);
	assert(getStackSize(stack) == 2);

	pushToStack(stack, &values[2]);

	assert(isStackEmpty(stack) == false);
	assert(isStackFull(stack) == true);
	assert(getStackSize(stack) == 3);

	destroyStack(stack);
}

void testStack02(CuTest* tc) {
	Stack* stack = initStack(3);

	int values[] = {1,2,3,4,5,6,7,8,9,10};

	assert(isStackEmpty(stack) == true);
	assert(isStackFull(stack) == false);
	assert(getStackSize(stack) == 0);

	pushToStack(stack, &values[0]);
	pushToStack(stack, &values[1]);

	assert(isStackEmpty(stack) == false);
	assert(isStackFull(stack) == false);
	assert(getStackSize(stack) == 2);

	int* i = (int*)peekFromStack(stack);
	assert(*i == 2);
	assert(getStackSize(stack) == 2);

	i = popFromStack(stack);
	assert(*i == 2);
	assert(getStackSize(stack) == 1);

	i = popFromStack(stack);
	assert(*i == 1);
	assert(getStackSize(stack) == 0);

	destroyStack(stack);
}

void testStack03(CuTest* tc) {
	Stack* stack = initStack(8);

	int values[] = {1,2,3,4,5,6,7,8,9,10};

	assert(isStackEmpty(stack) == true);
	assert(isStackFull(stack) == false);
	assert(getStackSize(stack) == 0);

	pushToStack(stack, &values[0]);
	pushToStack(stack, &values[1]);
	pushToStack(stack, &values[2]);
	pushToStack(stack, &values[3]);
	pushToStack(stack, &values[4]);

	int sum = 0;
	ITERATE_THROUGH_STACK(stack, n, int*) {
		sum += *n;
	}

	critical("sum is %d", sum);
	assert(sum == 15);

	destroyStack(stack);
}

void testStack04(CuTest* tc) {
	Stack* stack = initStack(8);


	int sum = 0;
	ITERATE_THROUGH_STACK(stack, n, int*) {
		sum += *n;
	}

	assert(sum == 0);

	destroyStack(stack);
}

CuSuite* CuStackSuite() {
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, testStack01);
	SUITE_ADD_TEST(suite, testStack02);
	SUITE_ADD_TEST(suite, testStack03);
	SUITE_ADD_TEST(suite, testStack04);

	return suite;
}

