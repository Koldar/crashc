/*
 * heapTest.c
 *
 *  Created on: Jun 5, 2017
 *      Author: koldar
 */

#include <assert.h>
#include <stdlib.h>
#include "CuTest.h"
#include "heap.h"
#include "defaultFunctions.h"
#include "log.h"
#include <string.h>

///test init and destroy functions
void testinitHeap01(CuSuite* tc) {
	heap* h = initHeap(10, payloadDestructorNOP, integerValueOrderer);

	assert(isHeapEmpty(h));
	assert(getSizeOfHeap(h) == 0);
	assert(getMaxSizeOfHeap(h) == 10);

	destroyHeap(h);
}

///test if adding item will increase the size of heap
void testinsertItemInHeap01(CuSuite* tc) {
	heap* h = initHeap(10, payloadDestructorNOP, integerValueOrderer);

	insertItemInHeap(h, 5);

	assert(!isHeapEmpty(h));
	assert(getSizeOfHeap(h) == 1);
	assert(getMaxSizeOfHeap(h) == 10);

	insertItemInHeap(h, 6);

	assert(!isHeapEmpty(h));
	assert(getSizeOfHeap(h) == 2);
	assert(getMaxSizeOfHeap(h) == 10);

	destroyHeapWithElements(h);
}

///test destruction with elements
void testinsertItemInHeap02(CuSuite* tc) {
	heap* h = initHeap(10, free, integerPointerOrderer);

	int* a = malloc(sizeof(int));
	*a = 5;
	insertItemInHeap(h, a);

	assert(!isHeapEmpty(h));
	assert(getSizeOfHeap(h) == 1);
	assert(getMaxSizeOfHeap(h) == 10);

	int* b = malloc(sizeof(int));
	*b = 6;
	insertItemInHeap(h, b);

	assert(!isHeapEmpty(h));
	assert(getSizeOfHeap(h) == 2);
	assert(getMaxSizeOfHeap(h) == 10);

	destroyHeapWithElements(h);
}

///test removeMinItemInHeap
void testinsertItemInHeap03(CuSuite* tc) {
	heap* h = initHeap(10, payloadDestructorNOP, integerValueOrderer);

	insertItemInHeap(h, 5);
	insertItemInHeap(h, 6);
	insertItemInHeap(h, 7);
	insertItemInHeap(h, 8);

	assert(!isHeapEmpty(h));
	assert(getSizeOfHeap(h) == 4);
	assert(getMaxSizeOfHeap(h) == 10);

	info("peek: %d", peekMinItemInHeap(h));
	assert(removeMinItemInHeap(h) == 5);

	info("peek: %d", peekMinItemInHeap(h));
	assert(removeMinItemInHeap(h) == 6);

	info("peek: %d", peekMinItemInHeap(h));
	assert(removeMinItemInHeap(h) == 7);

	info("peek: %d", peekMinItemInHeap(h));
	assert(removeMinItemInHeap(h) == 8);

	destroyHeapWithElements(h);
}

///like ::testinsertItemInHeap03 with unonrdered input
void testinsertItemInHeap04(CuSuite* tc) {
	heap* h = initHeap(10, payloadDestructorNOP, integerValueOrderer);

	insertItemInHeap(h, 7);
	insertItemInHeap(h, 8);
	insertItemInHeap(h, 5);
	insertItemInHeap(h, 6);

	assert(!isHeapEmpty(h));
	assert(getSizeOfHeap(h) == 4);
	assert(getMaxSizeOfHeap(h) == 10);

	info("peek: %d", peekMinItemInHeap(h));
	assert(removeMinItemInHeap(h) == 5);

	info("peek: %d", peekMinItemInHeap(h));
	assert(removeMinItemInHeap(h) == 6);

	info("peek: %d", peekMinItemInHeap(h));
	assert(removeMinItemInHeap(h) == 7);

	info("peek: %d", peekMinItemInHeap(h));
	assert(removeMinItemInHeap(h) == 8);

	destroyHeapWithElements(h);
}

///like maximum capacity
void testinsertItemInHeap05(CuSuite* tc) {
	heap* h = initHeap(5, payloadDestructorNOP, integerValueOrderer);

	assert(insertItemInHeap(h, 1) == true);
	assert(insertItemInHeap(h, 2) == true);
	assert(insertItemInHeap(h, 3) == true);
	assert(insertItemInHeap(h, 4) == true);
	assert(insertItemInHeap(h, 5) == true);

	assert(getSizeOfHeap(h) == 5);
	assert(getMaxSizeOfHeap(h) == 5);
	assert(isHeapFull(h));

	assert(insertItemInHeap(h, 6) == false);

	assert(getSizeOfHeap(h) == 5);
	assert(getMaxSizeOfHeap(h) == 5);
	assert(isHeapFull(h));

	destroyHeapWithElements(h);
}

void testClear(CuSuite* tc) {
	heap* h = initHeap(5, payloadDestructorNOP, integerValueOrderer);

	assert(insertItemInHeap(h, 1) == true);
	assert(insertItemInHeap(h, 2) == true);
	assert(insertItemInHeap(h, 3) == true);
	assert(insertItemInHeap(h, 4) == true);
	assert(insertItemInHeap(h, 5) == true);

	clearHeap(h);

	assert(getSizeOfHeap(h) == 0);
	assert(getMaxSizeOfHeap(h) == 5);
	assert(!isHeapFull(h));
	assert(isHeapEmpty(h));

	assert(insertItemInHeap(h, 4) == true);
	assert(insertItemInHeap(h, 3) == true);
	assert(insertItemInHeap(h, 5) == true);

	assert(getSizeOfHeap(h) == 3);
	assert(getMaxSizeOfHeap(h) == 5);
	assert(!isHeapFull(h));
	assert(!isHeapEmpty(h));

	assert(peekMinItemInHeap(h) == 3);
	assert(removeMinItemInHeap(h) == 3);
	assert(peekMinItemInHeap(h) == 4);
	assert(removeMinItemInHeap(h) == 4);
	assert(peekMinItemInHeap(h) == 5);
	assert(removeMinItemInHeap(h) == 5);

	destroyHeapWithElements(h);
}

void testcontainsItemInHeap01(CuTest* tc) {
	heap* h = initHeap(10, payloadDestructorNOP, integerValueOrderer);

	insertItemInHeap(h, 0);
	insertItemInHeap(h, 5);
	insertItemInHeap(h, 7);
	insertItemInHeap(h, -5);
	insertItemInHeap(h, 3);
	insertItemInHeap(h, 2);
	insertItemInHeap(h, 4);

	//check if root is present
	assert(containsItemInHeap(h, -5));
	assert(containsItemInHeap(h, 7));
	assert(containsItemInHeap(h, 0));
	assert(containsItemInHeap(h, -5));
	assert(!containsItemInHeap(h, -4));

	destroyHeapWithElements(h);
}

void testIterationOnHeap(CuTest* tc) {
	heap* h = initHeap(10, payloadDestructorNOP, integerValueOrderer);

	insertItemInHeap(h, 0);
	insertItemInHeap(h, 5);
	insertItemInHeap(h, 7);
	insertItemInHeap(h, -5);
	insertItemInHeap(h, 3);
	insertItemInHeap(h, 2);
	insertItemInHeap(h, 4);

	int sum = 0;
	ITERATE_THROUGH_HEAP(h, num1, int) {
		sum += num1;
	}
	assert(sum == 16);

	clearHeap(h);

	int flag = 0;
	ITERATE_THROUGH_HEAP(h, num2, int) {
		flag++;
	}
	assert(flag == 0);

	destroyHeapWithElements(h);
}

void testBufferStringOfHeap(CuTest* tc) {
	heap* h = initHeap(10, payloadDestructorNOP, integerValueOrderer);
	setBufferStringFunctionInHeap(h, defaultBufferString);

	insertItemInHeap(h, 0);
	insertItemInHeap(h, 5);
	insertItemInHeap(h, 7);
	insertItemInHeap(h, -5);
	insertItemInHeap(h, 3);
	insertItemInHeap(h, 2);
	insertItemInHeap(h, 4);

	char buffer[BUFFER_SIZE];
	char expected[BUFFER_SIZE] = "{ 0xfffffffffffffffb (nil) 0x2 0x5 0x3 0x7 0x4 }";
	getBufferStringOfHeap(h, buffer);
	info("actual is %s", buffer);
	assert(strcmp(buffer, expected) == 0);

	clearHeap(h);

	char expected2[BUFFER_SIZE] = "{ }";
	getBufferStringOfHeap(h, buffer);
	info("actual is %s", buffer);
	assert(strcmp(buffer, expected2) == 0);

	destroyHeapWithElements(h);
}

///dst is empty
void testcloneHeap01(CuTest* tc) {
	heap* h = initHeap(10, payloadDestructorNOP, integerValueOrderer);
	setBufferStringFunctionInHeap(h, defaultBufferString);
	setClonerFunctionInHeap(h, defaultPayloadCloner);

	insertItemInHeap(h, 0);
	insertItemInHeap(h, 5);
	insertItemInHeap(h, 7);
	insertItemInHeap(h, -5);
	insertItemInHeap(h, 3);
	insertItemInHeap(h, 2);
	insertItemInHeap(h, 4);

	heap* cloned = cloneHeap(h);

	assert(containsItemInHeap(h, 0));
	assert(containsItemInHeap(h, 5));
	assert(containsItemInHeap(h, 7));
	assert(containsItemInHeap(h, -5));
	assert(containsItemInHeap(h, 3));
	assert(containsItemInHeap(h, 2));
	assert(containsItemInHeap(h, 4));

	debug("clone has %d elements", getSizeOfHeap(cloned));
	assert(getSizeOfHeap(cloned) == 7);
	assert(containsItemInHeap(cloned, 0));
	assert(containsItemInHeap(cloned, 5));
	assert(containsItemInHeap(cloned, 7));
	assert(containsItemInHeap(cloned, -5));
	assert(containsItemInHeap(cloned, 3));
	assert(containsItemInHeap(cloned, 2));
	assert(containsItemInHeap(cloned, 4));

	destroyHeap(h);
	destroyHeap(cloned);
}

///dst is empty
void testmoveHeapElements01(CuTest* tc) {
	heap* h = initHeap(10, payloadDestructorNOP, integerValueOrderer);
	heap* dst = initHeap(10, payloadDestructorNOP, integerValueOrderer);
	setBufferStringFunctionInHeap(h, defaultBufferString);

	insertItemInHeap(h, 0);
	insertItemInHeap(h, 5);
	insertItemInHeap(h, 7);
	insertItemInHeap(h, -5);
	insertItemInHeap(h, 3);
	insertItemInHeap(h, 2);
	insertItemInHeap(h, 4);

	assert(isHeapEmpty(dst));
	assert(getSizeOfHeap(h) == 7);

	moveHeapElements(dst, h);

	assert(isHeapEmpty(h));
	assert(getSizeOfHeap(dst) == 7);

	assert(containsItemInHeap(dst, 0));
	assert(containsItemInHeap(dst, 5));
	assert(containsItemInHeap(dst, 7));
	assert(containsItemInHeap(dst, -5));
	assert(containsItemInHeap(dst, 3));
	assert(containsItemInHeap(dst, 2));
	assert(containsItemInHeap(dst, 4));

	destroyHeap(h);
	destroyHeap(dst);
}

///dst is not empty
void testmoveHeapElements02(CuTest* tc) {
	heap* h = initHeap(10, payloadDestructorNOP, integerValueOrderer);
	heap* dst = initHeap(10, payloadDestructorNOP, integerValueOrderer);
	setBufferStringFunctionInHeap(h, defaultBufferString);

	insertItemInHeap(dst, 10);
	insertItemInHeap(dst, 12);

	insertItemInHeap(h, 0);
	insertItemInHeap(h, 5);
	insertItemInHeap(h, 7);
	insertItemInHeap(h, -5);
	insertItemInHeap(h, 3);
	insertItemInHeap(h, 2);
	insertItemInHeap(h, 4);

	assert(getSizeOfHeap(dst) == 2);
	assert(getSizeOfHeap(h) == 7);

	moveHeapElements(dst, h);

	assert(isHeapEmpty(h));
	assert(getSizeOfHeap(dst) == 9);

	assert(containsItemInHeap(dst, 0));
	assert(containsItemInHeap(dst, 5));
	assert(containsItemInHeap(dst, 7));
	assert(containsItemInHeap(dst, -5));
	assert(containsItemInHeap(dst, 3));
	assert(containsItemInHeap(dst, 2));
	assert(containsItemInHeap(dst, 4));
	assert(containsItemInHeap(dst, 10));
	assert(containsItemInHeap(dst, 12));

	destroyHeap(h);
	destroyHeap(dst);
}

CuSuite* CuHeapSuite() {
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, testinitHeap01);
	SUITE_ADD_TEST(suite, testinsertItemInHeap01);
	SUITE_ADD_TEST(suite, testinsertItemInHeap02);
	SUITE_ADD_TEST(suite, testinsertItemInHeap03);
	SUITE_ADD_TEST(suite, testinsertItemInHeap04);
	SUITE_ADD_TEST(suite, testinsertItemInHeap05);
	SUITE_ADD_TEST(suite, testClear);
	SUITE_ADD_TEST(suite, testcontainsItemInHeap01);
	SUITE_ADD_TEST(suite, testBufferStringOfHeap);
	SUITE_ADD_TEST(suite, testcloneHeap01);
	SUITE_ADD_TEST(suite, testmoveHeapElements01);
	SUITE_ADD_TEST(suite, testmoveHeapElements02);


	return suite;
}

