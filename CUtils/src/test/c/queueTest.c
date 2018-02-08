/*
 * queueTest.c
 *
 *  Created on: Aug 4, 2017
 *      Author: koldar
 */



#include "CuTest.h"
#include <assert.h>
#include "queue.h"
#include "defaultFunctions.h"



void testQueue01(CuTest* tc) {
	queue* q = initQueue(defaultEvaluator, false, payloadDestructorNOP);

	destroyQueue(q);
}

void testQueue02(CuTest* tc) {
	queue* q = initQueue(defaultEvaluator, false, payloadDestructorNOP);

	assert(isQueueEmpty(q));
	assert(getSizeOfQueue(q) == 0);
	addItemInQueue(q, 4);
	assert(!isQueueEmpty(q));
	assert(getSizeOfQueue(q) == 1);

	destroyQueue(q);
}

///popping and peeking a single element queue
void testQueue03(CuTest* tc) {
	queue* q = initQueue(defaultEvaluator, false, payloadDestructorNOP);

	addItemInQueue(q, 4);

	assert(peekItemFromQueue(q, int) == 4);
	assert(!isQueueEmpty(q));
	assert(getSizeOfQueue(q) == 1);
	assert(popItemFromQueue(q, int) == 4);

	assert(isQueueEmpty(q));
	assert(getSizeOfQueue(q) == 0);
	assert(peekItemFromQueue(q, void*) == NULL);

	destroyQueue(q);
}

///Adding several values in a queue
void testQueue04(CuTest* tc) {
	queue* q = initQueue(defaultEvaluator, false, payloadDestructorNOP);

	addItemInQueue(q, 4);
	addItemInQueue(q, 3);
	addItemInQueue(q, 6);
	addItemInQueue(q, 10);

	assert(peekItemFromQueue(q, int) == 3);
	assert(!isQueueEmpty(q));
	assert(getSizeOfQueue(q) == 4);

	destroyQueue(q);
}

///popping and peeking queue with more values
void testQueue05(CuTest* tc) {
	queue* q = initQueue(defaultEvaluator, false, payloadDestructorNOP);

	addItemInQueue(q, 4);
	addItemInQueue(q, 3);
	addItemInQueue(q, 6);
	addItemInQueue(q, 10);

	assert(peekItemFromQueue(q, int) == 3);
	assert(!isQueueEmpty(q));
	assert(getSizeOfQueue(q) == 4);
	assert(popItemFromQueue(q, int) == 3);
	assert(peekItemFromQueue(q, int) == 4);
	assert(!isQueueEmpty(q));
	assert(getSizeOfQueue(q) == 3);

	destroyQueue(q);
}

///popping and peeking queue with more values
void testQueue06(CuTest* tc) {
	queue* q = initQueue(defaultEvaluator, false, payloadDestructorNOP);

	addItemInQueue(q, 4);
	addItemInQueue(q, 3);
	addItemInQueue(q, 10);
	addItemInQueue(q, 6);

	assert(peekItemFromQueue(q, int) == 3);
	assert(!isQueueEmpty(q));
	assert(getSizeOfQueue(q) == 4);
	assert(popItemFromQueue(q, int) == 3);
	assert(popItemFromQueue(q, int) == 4);
	assert(!isQueueEmpty(q));
	assert(getSizeOfQueue(q) == 2);

	//add elements that won't alter min
	assert(peekItemFromQueue(q, int) == 6);
	addItemInQueue(q, 13);
	addItemInQueue(q, 11);
	assert(!isQueueEmpty(q));
	assert(getSizeOfQueue(q) == 4);

	assert(peekItemFromQueue(q, int) == 6);
	assert(popItemFromQueue(q, int) == 6);
	assert(peekItemFromQueue(q, int) == 10);
	assert(!isQueueEmpty(q));
	assert(getSizeOfQueue(q) == 3);

	//add element that will alter min
	addItemInQueue(q, 5);
	addItemInQueue(q, 3);
	assert(peekItemFromQueue(q, int) == 3);
	assert(!isQueueEmpty(q));
	assert(getSizeOfQueue(q) == 5);

	destroyQueue(q);
}

CuSuite* CuQueueSuite() {
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, testQueue01);
	SUITE_ADD_TEST(suite, testQueue02);
	SUITE_ADD_TEST(suite, testQueue03);
	SUITE_ADD_TEST(suite, testQueue04);
	SUITE_ADD_TEST(suite, testQueue05);
	SUITE_ADD_TEST(suite, testQueue06);

	return suite;
}
