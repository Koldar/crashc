/*
 * redBlackTreetest.c
 *
 *  Created on: Jun 9, 2017
 *      Author: koldar
 */

#include <stdlib.h>

#include "log.h"
#include "CuTest.h"
#include <assert.h>
#include "redBlackTree.h"
#include "defaultFunctions.h"

void testRedBlackTree01(CuTest* tc) {
	rb_tree* tree = initRedBlackTree(payloadDestructorNOP, integerValueOrderer);
	destroyRedBlackTree(tree);
}

void testRedBlackTree02(CuTest* tc) {
	rb_tree* tree = initRedBlackTree(payloadDestructorNOP, integerValueOrderer);

	assert(getSizeOfRedBlackTree(tree) == 0);
	assert(isRedBlackTreeEmpty(tree));
	assert(containsItemInRedBlackTree(tree, 5) == false);
	assert(containsItemInRedBlackTree(tree, 3) == false);

	addItemInRedBlackTree(tree, 5);

	assert(getSizeOfRedBlackTree(tree) == 1);
	assert(!isRedBlackTreeEmpty(tree));
	assert(containsItemInRedBlackTree(tree, 5) == true);
	assert(containsItemInRedBlackTree(tree, 3) == false);

	destroyRedBlackTreeWithElements(tree);
}

///test additional adds
void testRedBlackTree03(CuTest* tc) {
	rb_tree* tree = initRedBlackTree(payloadDestructorNOP, integerValueOrderer);

	assert(getSizeOfRedBlackTree(tree) == 0);
	assert(isRedBlackTreeEmpty(tree));

	int memory[10];
	int t;
	for (int i=0; i<100; i++) {
		t = rand()%50;
		if ((i % 10) == 0) {
			memory[i/10] = t;
		}
		addItemInRedBlackTree(tree, t);
	}

	assert(getSizeOfRedBlackTree(tree) == 100);
	assert(!isRedBlackTreeEmpty(tree));
	for (int i=0; i<10; i++) {
		assert(containsItemInRedBlackTree(tree, memory[i]));
	}

	destroyRedBlackTreeWithElements(tree);
}

///test minimum
void testRedBlackTree04(CuTest* tc) {
	rb_tree* tree = initRedBlackTree(payloadDestructorNOP, integerValueOrderer);

	assert(getSizeOfRedBlackTree(tree) == 0);
	assert(isRedBlackTreeEmpty(tree));

	int t;
	for (int i=0; i<100; i++) {
		t = rand()%50 + 5;
		addItemInRedBlackTree(tree, t);
	}
	addItemInRedBlackTree(tree, 3);

	assert(getSizeOfRedBlackTree(tree) == 101);
	assert(!isRedBlackTreeEmpty(tree));
	int min = getMinimumInRedBlackTree(tree);
	info("the minimum is %d");
	assert(min == 3);

	destroyRedBlackTreeWithElements(tree);
}

///test minimum in empty tree
void testRedBlackTree05(CuTest* tc) {
	rb_tree* tree = initRedBlackTree(payloadDestructorNOP, integerValueOrderer);

	assert(getSizeOfRedBlackTree(tree) == 0);
	assert(isRedBlackTreeEmpty(tree));
	assert(getMinimumInRedBlackTree(tree) == NULL);

	destroyRedBlackTreeWithElements(tree);
}

///test removal
void testRedBlackTree06(CuTest* tc) {
	rb_tree* tree = initRedBlackTree(payloadDestructorNOP, integerValueOrderer);

	addItemInRedBlackTree(tree, 5);
	addItemInRedBlackTree(tree, 4);
	addItemInRedBlackTree(tree, 7);
	addItemInRedBlackTree(tree, 2);
	addItemInRedBlackTree(tree, 8);

	assert(getSizeOfRedBlackTree(tree) == 5);

	assert(removeItemInRedBlackTree(tree, 1) == false);
	assert(getSizeOfRedBlackTree(tree) == 5);

	assert(removeItemInRedBlackTree(tree, 2) == true);
	assert(getSizeOfRedBlackTree(tree) == 4);
	assert(containsItemInRedBlackTree(tree, 2) == false);
	assert(containsItemInRedBlackTree(tree, 4));
	assert(containsItemInRedBlackTree(tree, 5));
	assert(containsItemInRedBlackTree(tree, 7));
	assert(containsItemInRedBlackTree(tree, 8));

	assert(removeItemInRedBlackTree(tree, 5) == true);
	assert(getSizeOfRedBlackTree(tree) == 3);
	assert(containsItemInRedBlackTree(tree, 2) == false);
	assert(containsItemInRedBlackTree(tree, 4));
	assert(containsItemInRedBlackTree(tree, 5) == false);
	assert(containsItemInRedBlackTree(tree, 7));
	assert(containsItemInRedBlackTree(tree, 8));

	assert(removeItemInRedBlackTree(tree, 8) == true);
	assert(getSizeOfRedBlackTree(tree) == 2);
	assert(containsItemInRedBlackTree(tree, 2) == false);
	assert(containsItemInRedBlackTree(tree, 4));
	assert(containsItemInRedBlackTree(tree, 5) == false);
	assert(containsItemInRedBlackTree(tree, 7));
	assert(containsItemInRedBlackTree(tree, 8) == false);

	assert(removeItemInRedBlackTree(tree, 4) == true);
	assert(getSizeOfRedBlackTree(tree) == 1);
	assert(containsItemInRedBlackTree(tree, 2) == false);
	assert(containsItemInRedBlackTree(tree, 4) == false);
	assert(containsItemInRedBlackTree(tree, 5) == false);
	assert(containsItemInRedBlackTree(tree, 7));
	assert(containsItemInRedBlackTree(tree, 8) == false);

	assert(removeItemInRedBlackTree(tree, 7) == true);
	assert(getSizeOfRedBlackTree(tree) == 0);
	assert(containsItemInRedBlackTree(tree, 2) == false);
	assert(containsItemInRedBlackTree(tree, 4) == false);
	assert(containsItemInRedBlackTree(tree, 5) == false);
	assert(containsItemInRedBlackTree(tree, 7) == false);
	assert(containsItemInRedBlackTree(tree, 8) == false);

	assert(removeItemInRedBlackTree(tree, 7) == false);

	destroyRedBlackTreeWithElements(tree);
}

//some insertion and removal in the red black tree
void testRedBlackTree07(CuTest* tc) {
	rb_tree* tree = initRedBlackTree(payloadDestructorNOP, integerValueOrderer);

	addItemInRedBlackTree(tree, 5);
	addItemInRedBlackTree(tree, 4);
	addItemInRedBlackTree(tree, 7);

	assert(!removeItemInRedBlackTree(tree, 3));
	assert(removeItemInRedBlackTree(tree, 4));

	assert(containsItemInRedBlackTree(tree, 4) == false);
	addItemInRedBlackTree(tree, 4);
	assert(containsItemInRedBlackTree(tree, 4) == true);

	assert(removeItemInRedBlackTree(tree, 7));
	assert(removeItemInRedBlackTree(tree, 5));
	assert(removeItemInRedBlackTree(tree, 4));

	assert(!removeItemInRedBlackTree(tree, 4));
	assert(!removeItemInRedBlackTree(tree, 5));
	assert(!removeItemInRedBlackTree(tree, 7));


	addItemInRedBlackTree(tree, 2);
	addItemInRedBlackTree(tree, 8);

	assert(containsItemInRedBlackTree(tree, 2) == true);
	assert(containsItemInRedBlackTree(tree, 8) == true);

	destroyRedBlackTreeWithElements(tree);
}


CuSuite* CuRedBlackTreeSuite() {
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, testRedBlackTree01);
	SUITE_ADD_TEST(suite, testRedBlackTree02);
	SUITE_ADD_TEST(suite, testRedBlackTree03);
	SUITE_ADD_TEST(suite, testRedBlackTree04);
	SUITE_ADD_TEST(suite, testRedBlackTree05);
	SUITE_ADD_TEST(suite, testRedBlackTree06);
	SUITE_ADD_TEST(suite, testRedBlackTree07);

	return suite;
}

