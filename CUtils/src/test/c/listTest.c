/*
 * RegexTest.c
 *
 *  Created on: Dec 23, 2016
 *      Author: koldar
 */

#include "CuTest.h"
#include "list.h"
#include <string.h>
#include <assert.h>
#include "macros.h"
#include "defaultFunctions.h"
#include "log.h"


static int testFindNumberContext = 0;
static testFindNumberLambda(int* c) {
	return *c == testFindNumberContext;
}

void testList01(CuSuite* suite) {
	list* l = initList();
	destroyList(l);
}

void testList02(CuSuite* suite) {
	list* l = initList();

	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;

	assert(isEmptyList(l) == true);
	assert(getLengthOfList(l) == 0);
	addHeadInList(l, &a);
	assert(isEmptyList(l) == false);
	assert(getLengthOfList(l) == 1);
	addHeadInList(l, &b);
	assert(isEmptyList(l) == false);
	assert(getLengthOfList(l) == 2);
	addHeadInList(l, &c);
	assert(isEmptyList(l) == false);
	assert(getLengthOfList(l) == 3);
	addHeadInList(l, &d);
	assert(isEmptyList(l) == false);
	assert(getLengthOfList(l) == 4);

	destroyList(l);
}

void testList03(CuSuite* suite) {
	list* l = initList();

	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;

	assert(isEmptyList(l) == true);
	assert(getLengthOfList(l) == 0);
	addTailInList(l, &a);
	assert(isEmptyList(l) == false);
	assert(getLengthOfList(l) == 1);
	addTailInList(l, &b);
	assert(isEmptyList(l) == false);
	assert(getLengthOfList(l) == 2);
	addTailInList(l, &c);
	assert(isEmptyList(l) == false);
	assert(getLengthOfList(l) == 3);
	addTailInList(l, &d);
	assert(isEmptyList(l) == false);
	assert(getLengthOfList(l) == 4);

	destroyList(l);
}

void testList04(CuSuite* suite) {
	list* lst = initList();

	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;

	addTailInList(lst, &a);
	addTailInList(lst, &b);
	addTailInList(lst, &c);
	addTailInList(lst, &d);

	int sum = 0;
	ITERATE_ON_LIST(lst, cell, payload, int) {
		sum += *payload;
	}
	assert(sum == 10);

	destroyList(lst);
}

void testList05(CuSuite* suite) {
	list* lst = initList();

	int* popped = 0;
	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;

	addTailInList(lst, &a);
	addTailInList(lst, &b);
	addTailInList(lst, &c);
	addTailInList(lst, &d);

	popped = popFromList(lst);
	assert(getLengthOfList(lst) == 3);
	assert(*popped == 1);
	popped = popFromList(lst);
	assert(getLengthOfList(lst) == 2);
	assert(*popped == 2);
	popped = popFromList(lst);
	assert(getLengthOfList(lst) == 1);
	assert(*popped == 3);
	popped = popFromList(lst);
	assert(getLengthOfList(lst) == 0);
	assert(*popped == 4);
	popped = popFromList(lst);
	assert(getLengthOfList(lst) == 0);
	assert(popped == NULL);

	destroyList(lst);
}

void testList06(CuSuite* suite) {
	list* lst = initList();

	int* popped = 0;
	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;

	addTailInList(lst, &a);
	addTailInList(lst, &b);
	addTailInList(lst, &c);
	addTailInList(lst, &d);

	int* value = NULL;
	testFindNumberContext = 3;
	value = findElementInList(lst, testFindNumberLambda);
	assert((*value) == 3);

	testFindNumberContext = 5;
	value = findElementInList(lst, testFindNumberLambda);
	printf("value is %p\n", value);
	assert(value == NULL);

	destroyList(lst);
}

void testList07(CuSuite* suite) {
	list* lst = initList();

	int* popped = 0;
	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;

	addTailInList(lst, &a);
	addTailInList(lst, &b);
	addTailInList(lst, &c);
	addTailInList(lst, &d);

	testFindNumberContext = 5;
	removeElementInList(lst, testFindNumberLambda);
	assert(getLengthOfList(lst) == 4);

	destroyList(lst);
}

void testList08(CuSuite* suite) {
	list* lst = initList();

	int* popped = 0;
	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;

	addTailInList(lst, &a);
	addTailInList(lst, &b);
	addTailInList(lst, &c);
	addTailInList(lst, &d);


	testFindNumberContext = 1;
	removeElementInList(lst, testFindNumberLambda);
	assert(getLengthOfList(lst) == 3);
	int sum = 0;
	ITERATE_ON_LIST(lst, cell, value, int) {
		sum += *value;
	}
	assert(sum == 9);

	destroyList(lst);
}

void testList09(CuSuite* suite) {
	list* lst = initList();

	int* popped = 0;
	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;

	addTailInList(lst, &a);
	addTailInList(lst, &b);
	addTailInList(lst, &c);
	addTailInList(lst, &d);


	testFindNumberContext = 4;
	removeElementInList(lst, testFindNumberLambda);
	assert(getLengthOfList(lst) == 3);
	int sum = 0;
	ITERATE_ON_LIST(lst, cell, value, int) {
		sum += *value;
	}
	assert(sum == 6);

	destroyList(lst);
}

void testList10(CuSuite* suite) {
	list* lst = initList();

	int* popped = 0;
	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;

	addTailInList(lst, &a);
	addTailInList(lst, &b);
	addTailInList(lst, &c);
	addTailInList(lst, &d);


	testFindNumberContext = 3;
	removeElementInList(lst, testFindNumberLambda);
	assert(getLengthOfList(lst) == 3);
	int sum = 0;
	ITERATE_ON_LIST(lst, cell, value, int) {
		sum += *value;
	}
	assert(sum == 7);

	destroyList(lst);
}

//test clearList
void testList11(CuSuite* suite) {
	list* lst = initList();

	int* popped = 0;
	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;

	assert(getLengthOfList(lst) == 0);
	clearList(lst);
	assert(getLengthOfList(lst) == 0);

	addTailInList(lst, &a);
	addTailInList(lst, &b);
	addTailInList(lst, &c);
	addTailInList(lst, &d);


	assert(getLengthOfList(lst) == 4);
	clearList(lst);
	assert(getLengthOfList(lst) == 0);

	addHeadInList(lst, &a);
	addHeadInList(lst, &b);
	addHeadInList(lst, &c);
	addHeadInList(lst, &d);

	assert(getLengthOfList(lst) == 4);

	clearList(lst);
	assert(getLengthOfList(lst) == 0);

	destroyList(lst);
}

static int numToRemove;
static bool lambdaNumToRemove(int* a) {
	return *a == numToRemove;
}

///test remove element from list on head
void testList12(CuTest* tc) {
	int sum = 0;
	list* lst = initList();

	int* popped = 0;
	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;

	addTailInList(lst, &a);
	addTailInList(lst, &b);
	addTailInList(lst, &c);
	addTailInList(lst, &d);

	assert(getLengthOfList(lst) == 4);
	sum = 0;
	ITERATE_ON_LIST(lst, cell, n, int) {
		sum += (int)*n;
	}
	assert(sum == 10);

	//remove head
	numToRemove = 1;
	removeElementInList(lst, lambdaNumToRemove);

	assert(getLengthOfList(lst) == 3);
	sum = 0;
	ITERATE_ON_LIST(lst, cell1, n1, int) {
		sum += (int)*n1;
	}
	assert(sum == 9);

	destroyList(lst);
}

///test remove element from list on tail
void testList13(CuTest* tc) {
	int sum = 0;
	list* lst = initList();

	int* popped = 0;
	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;

	addTailInList(lst, &a);
	addTailInList(lst, &b);
	addTailInList(lst, &c);
	addTailInList(lst, &d);

	assert(getLengthOfList(lst) == 4);
	sum = 0;
	ITERATE_ON_LIST(lst, cell, n, int) {
		sum += (int)*n;
	}
	assert(sum == 10);

	//remove head
	numToRemove = 4;
	removeElementInList(lst, lambdaNumToRemove);

	assert(getLengthOfList(lst) == 3);
	sum = 0;
	ITERATE_ON_LIST(lst, cell1, n1, int) {
		sum += (int)*n1;
	}
	assert(sum == 6);

	destroyList(lst);
}

///test remove element from list in middle
void testList14(CuTest* tc) {
	int sum = 0;
	list* lst = initList();

	int* popped = 0;
	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;

	addTailInList(lst, &a);
	addTailInList(lst, &b);
	addTailInList(lst, &c);
	addTailInList(lst, &d);

	assert(getLengthOfList(lst) == 4);
	sum = 0;
	ITERATE_ON_LIST(lst, cell, n, int) {
		sum += (int)*n;
	}
	assert(sum == 10);

	//remove head
	numToRemove = 3;
	removeElementInList(lst, lambdaNumToRemove);

	assert(getLengthOfList(lst) == 3);
	sum = 0;
	ITERATE_ON_LIST(lst, cell1, n1, int) {
		sum += (int)*n1;
	}
	assert(sum == 7);

	destroyList(lst);
}

//test ::getLastElementOfList
void testGetLastElementOfList(CuSuite* suite) {
	list* lst = initList();

	int* popped = 0;
	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;

	assert(getLastElementOfList(lst) == NULL);

	addTailInList(lst, &a);
	assert(getLastElementOfList(lst) == &a);
	addHeadInList(lst, &b);
	assert(getLastElementOfList(lst) == &a);
	addTailInList(lst, &c);
	assert(getLastElementOfList(lst) == &c);

	clearList(lst);

	assert(getLastElementOfList(lst) == NULL);

	destroyList(lst);
}

void testRemoval(CuSuite* suite) {
	list* lst = initList();

	int* a = malloc(sizeof(int)); *a = 1;
	int* b = malloc(sizeof(int)); *b = 2;
	int* c = malloc(sizeof(int)); *c = 3;
	int* d = malloc(sizeof(int)); *d = 4;

	addTailInList(lst, a);
	addTailInList(lst, b);
	addTailInList(lst, c);
	addTailInList(lst, d);


	testFindNumberContext = 3;
	assert(removeAndDestroyElementInList(lst,testFindNumberLambda, free) == true);
	assert(getLengthOfList(lst) == 3);
	testFindNumberContext = 3;
	assert(removeAndDestroyElementInList(lst,testFindNumberLambda, free) == false);

	destroyListWithElement(lst, free);
}

void testgetStringOfList01(CuSuite* tc) {
	list* lst = initList();

	int* a = malloc(sizeof(int)); *a = 1;
	int* b = malloc(sizeof(int)); *b = 2;
	int* c = malloc(sizeof(int)); *c = 3;
	int* d = malloc(sizeof(int)); *d = 4;

	addTailInList(lst, a);
	addTailInList(lst, b);
	addTailInList(lst, c);
	addTailInList(lst, d);

	char* toString = getStringOfList(lst, integerPointerStringify);
	info("%s", toString);
	assert(strcmp(toString, "[1, 2, 3, 4]") == 0);
	free(toString);

	clearList(lst);

	toString = getStringOfList(lst, integerPointerStringify);
	info("%s", toString);
	assert(strcmp(toString, "[]") == 0);
	free(toString);

	assert(getLengthOfList(lst) == 0);
	destroyListWithElement(lst, free);

	free(a);
	free(b);
	free(c);
	free(d);

	info("DONE deleting list");
}

void testgetNthElementOfList01(CuSuite* tc) {
	list* lst = initList();

	int* a = malloc(sizeof(int)); *a = 1;
	int* b = malloc(sizeof(int)); *b = 2;
	int* c = malloc(sizeof(int)); *c = 3;
	int* d = malloc(sizeof(int)); *d = 4;

	addTailInList(lst, a);
	addTailInList(lst, b);
	addTailInList(lst, c);
	addTailInList(lst, d);

	assert(getNthElementOfList(lst, 0) == a);
	assert(getNthElementOfList(lst, 1) == b);
	assert(getNthElementOfList(lst, 2) == c);
	assert(getNthElementOfList(lst, 3) == d);
	assert(getNthElementOfList(lst, -1) == NULL);
	assert(getNthElementOfList(lst, 4) == NULL);

	destroyListWithElement(lst, free);
}

void testremoveElementInListCell01(CuSuite* tc) {
	list* lst = initList();

	int* a = malloc(sizeof(int)); *a = 1;
	int* b = malloc(sizeof(int)); *b = 2;
	int* c = malloc(sizeof(int)); *c = 3;
	int* d = malloc(sizeof(int)); *d = 4;

	addTailInList(lst, a);
	addTailInList(lst, b);
	addTailInList(lst, c);
	addTailInList(lst, d);

	int sum = 0;
	VARIABLE_ITERATE_ON_LIST(lst, prevCell, nowCell, number1, int) {
		sum += *number1;
	}
	assert(sum == 10);
	assert(getLengthOfList(lst) == 4);

	sum = 0;
	VARIABLE_ITERATE_ON_LIST(lst, prevCell, nowCell, number2, int) {
		info("prevcell: %p; nowCell: %p", prevCell, nowCell);
		if (*number2 <= 2) {
			removeElementInListCell(lst, &prevCell, nowCell);
		} else {
			sum += *number2;
		}
	}
	assert(sum == 7);
	assert(getLengthOfList(lst) == 2);

	sum = 0;
	VARIABLE_ITERATE_ON_LIST(lst, prevCell, nowCell, number3, int) {
		if (*number3 > 2) {
			removeElementInListCell(lst, &prevCell, nowCell);
		} else {
			sum += *number3;
		}
	}
	assert(sum == 0);
	assert(getLengthOfList(lst) == 0);

	destroyList(lst);
	free(a);
	free(b);
	free(c);
	free(d);
}

void testmoveListContents(CuTest* tc) {
	list* a = initList();
	list* b = initList();

	addTailInList(a, 1);
	addTailInList(a, 2);
	addTailInList(a, 3);
	addTailInList(a, 4);

	addTailInList(b, 5);
	addTailInList(b, 6);
	addTailInList(b, 7);

	assert(getLengthOfList(a) == 4);
	assert(getLengthOfList(b) == 3);

	moveListContents(a, b);

	assert(getLengthOfList(a) == 7);
	assert(getLengthOfList(b) == 0);

	int sum;

	sum = 0;
	ITERATE_ON_LIST(a, cell1, num1, int) {
		sum += (int)num1;
	}
	info("sum is %d", sum);
	assert(sum == 28);

	sum = 0;
	ITERATE_ON_LIST(b, cell2, num2, int) {
		sum += (int) num2;
	}
	assert(sum == 0);

	addTailInList(b, 8);
	addTailInList(b, 9);
	addTailInList(b, 10);

	moveListContents(a, b);

	assert(getLengthOfList(a) == 10);
	assert(getLengthOfList(b) == 0);

	sum = 0;
	ITERATE_ON_LIST(a, cell3, num3, int) {
		sum += (int) num3;
	}
	assert(sum == 55);

	sum = 0;
	ITERATE_ON_LIST(b, cell4, num4, int) {
		sum += (int) num4;
	}
	assert(sum == 0);

	moveListContents(a, b);


	assert(getLengthOfList(a) == 10);
	assert(getLengthOfList(b) == 0);

	moveListContents(b, a);

	assert(getLengthOfList(a) == 0);
	assert(getLengthOfList(b) == 10);

	moveListContents(a, b);

	assert(getLengthOfList(a) == 10);
	assert(getLengthOfList(b) == 0);


	destroyList(a);
	destroyList(b);
}

void testVariableIterateOnList(CuTest* tc) {
	list* l = initList();
	int sum = 0;

	for (int i=0; i<9; i++) {
		addTailInList(l, i+1);
	}

	int j=0;
	{
		VARIABLE_ITERATE_ON_LIST(l, prevCell, cell, num, int) {
			j = (int)num;
			critical("j is %d", num);
			if (j == 1) {
				assert(prevCell == NULL);
			} else {
				assert(prevCell != NULL);
				assert((int)(prevCell->payload) == (j-1));
			}
		}
	}

	sum = 0;
	{
		ITERATE_ON_LIST(l, cell, num1, int) {
			sum += (int) num1;
		}
	}
	assert(sum == 45);

	//remove head
	{
		VARIABLE_ITERATE_ON_LIST(l, prevCell, cell, num2, int) {
			j = (int)num2;
			if (j == 1) {
				removeElementInListCell(l, &prevCell, cell);
			}
		}
	}

	sum = 0;
	{
		ITERATE_ON_LIST(l, cell, num3, int) {
			sum += (int) num3;
		}
	}
	assert(sum == 44);

	//remove tail
	{
		VARIABLE_ITERATE_ON_LIST(l, prevCell, cell, num4, int) {
			j = (int)num4;
			if (j == 9) {
				removeElementInListCell(l, &prevCell, cell);
			}
		}
	}

	sum = 0;
	{
		ITERATE_ON_LIST(l, cell, num5, int) {
			sum += (int) num5;
		}
		assert(sum == 35);
	}

	//remove something in between
	{
		VARIABLE_ITERATE_ON_LIST(l, prevCell, cell, num6, int) {
			j = (int)num6;
			if (j == 5) {
				removeElementInListCell(l, &prevCell, cell);
			}
		}
	}

	sum = 0;
	{
		ITERATE_ON_LIST(l, cell, num7, int) {
			sum += (int) num7;
		}
	}
	assert(sum == 30);

	//remove the second element
	{
		VARIABLE_ITERATE_ON_LIST(l, prevCell, cell, num6, int) {
			j = (int)num6;
			if (j == 3) {
				removeElementInListCell(l, &prevCell, cell);
			}
		}
	}

	sum = 0;
	{
		ITERATE_ON_LIST(l, cell, num8, int) {
			sum += (int) num8;
		}
	}
	assert(sum == 27);

	destroyList(l);
}


CuSuite* CuProjectSuite() {
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, testList01);
	SUITE_ADD_TEST(suite, testList02);
	SUITE_ADD_TEST(suite, testList03);
	SUITE_ADD_TEST(suite, testList04);
	SUITE_ADD_TEST(suite, testList05);
	SUITE_ADD_TEST(suite, testList06);
	SUITE_ADD_TEST(suite, testList07);
	SUITE_ADD_TEST(suite, testList08);
	SUITE_ADD_TEST(suite, testList09);
	SUITE_ADD_TEST(suite, testList10);
	SUITE_ADD_TEST(suite, testList11);
	SUITE_ADD_TEST(suite, testList12);
	SUITE_ADD_TEST(suite, testList13);
	SUITE_ADD_TEST(suite, testList14);
	SUITE_ADD_TEST(suite, testGetLastElementOfList);
	SUITE_ADD_TEST(suite, testRemoval);
	SUITE_ADD_TEST(suite, testgetStringOfList01);
	SUITE_ADD_TEST(suite, testgetNthElementOfList01);
	SUITE_ADD_TEST(suite, testremoveElementInListCell01);
	SUITE_ADD_TEST(suite, testmoveListContents);
	SUITE_ADD_TEST(suite, testVariableIterateOnList);

	return suite;
}
