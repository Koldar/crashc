/*
 * binarySearchTreeTest.c
 *
 *  Created on: Jun 6, 2017
 *      Author: koldar
 */

#include "CuTest.h"
#include "defaultFunctions.h"
#include "binaryTree.h"
#include "log.h"
#include <assert.h>

void testBinarySearchTree01(CuTest* tc) {
	binary_tree* bt = initBinaryTree(integerValueOrderer, payloadDestructorNOP);
	destroyBinaryTree(bt);
}

void testBinarySearchTree02(CuTest* tc) {
	binary_tree* bt = initBinaryTree(integerValueOrderer, payloadDestructorNOP);

	assert(getSizeOfBinaryTree(bt) == 0);
	addItemInBinaryTree(bt, 4);
	assert(getSizeOfBinaryTree(bt) == 1);

	destroyBinaryTree(bt);
}

void testBinarySearchTree03(CuTest* tc) {
	binary_tree* bt = initBinaryTree(integerValueOrderer, payloadDestructorNOP);

	assert(getSizeOfBinaryTree(bt) == 0);
	addItemInBinaryTree(bt, 4);
	addItemInBinaryTree(bt, 3);
	addItemInBinaryTree(bt, 5);
	addItemInBinaryTree(bt, 1);
	assert(getSizeOfBinaryTree(bt) == 4);

	destroyBinaryTree(bt);
}

void testBinarySearchTree04(CuTest* tc) {
	binary_tree* bt = initBinaryTree(integerValueOrderer, payloadDestructorNOP);

	info("orderer index is %p", integerValueOrderer);
	assert(getSizeOfBinaryTree(bt) == 0);
	addItemInBinaryTree(bt, 4);
	addItemInBinaryTree(bt, 3);
	addItemInBinaryTree(bt, 5);
	addItemInBinaryTree(bt, 1);
	assert(getSizeOfBinaryTree(bt) == 4);

	assert(containsItemInBinaryTree(bt, 6) == false);
	assert(containsItemInBinaryTree(bt, 0) == false);
	assert(containsItemInBinaryTree(bt, 1) == true);
	assert(containsItemInBinaryTree(bt, 3) == true);
	assert(containsItemInBinaryTree(bt, 4) == true);
	assert(containsItemInBinaryTree(bt, 5) == true);

	destroyBinaryTree(bt);
}

void testBinarySearchTree05(CuTest* tc) {
	char* str;
	binary_tree* bt = initBinaryTree(integerValueOrderer, payloadDestructorNOP);

	assert(getSizeOfBinaryTree(bt) == 0);
	addItemInBinaryTree(bt, 4);
	addItemInBinaryTree(bt, 3);
	addItemInBinaryTree(bt, 5);
	addItemInBinaryTree(bt, 1);
	assert(getSizeOfBinaryTree(bt) == 4);

	assert(removeItemInBinaryTree(bt, 7) == false);
	assert(getSizeOfBinaryTree(bt) == 4);
	//remove the root itself
	assert(removeItemInBinaryTree(bt, 4) == true);
	assert(getSizeOfBinaryTree(bt) == 3);
	assert(removeItemInBinaryTree(bt, 4) == false);
	assert(getSizeOfBinaryTree(bt) == 3);

	assert(removeItemInBinaryTree(bt, 3) == true);
	assert(getSizeOfBinaryTree(bt) == 2);
	assert(removeItemInBinaryTree(bt, 1) == true);
	assert(getSizeOfBinaryTree(bt) == 1);
	assert(removeItemInBinaryTree(bt, 5) == true);
	assert(getSizeOfBinaryTree(bt) == 0);

	destroyBinaryTree(bt);
}

void testBinarySearchTree06(CuTest* tc) {
	char* str;
	binary_tree* bt = initBinaryTree(integerValueOrderer, payloadDestructorNOP);

	assert(getSizeOfBinaryTree(bt) == 0);
	addItemInBinaryTree(bt, 4);
	addItemInBinaryTree(bt, 3);
	addItemInBinaryTree(bt, 5);
	addItemInBinaryTree(bt, 1);
	assert(getSizeOfBinaryTree(bt) == 4);

	assert(removeItemInBinaryTree(bt, 7) == false);
	assert(getSizeOfBinaryTree(bt) == 4);
	//remove the root itself
	assert(removeItemInBinaryTree(bt, 4) == true);
	assert(getSizeOfBinaryTree(bt) == 3);
	assert(removeItemInBinaryTree(bt, 4) == false);
	assert(getSizeOfBinaryTree(bt) == 3);

	assert(removeItemInBinaryTree(bt, 3) == true);
	assert(getSizeOfBinaryTree(bt) == 2);

	//adds new elements
	addItemInBinaryTree(bt, 7);
	assert(getSizeOfBinaryTree(bt) == 3);

	clearBinaryTree(bt);
	assert(getSizeOfBinaryTree(bt) == 0);

	assert(removeItemInBinaryTree(bt, 1) == false);
	assert(getSizeOfBinaryTree(bt) == 0);
	assert(removeItemInBinaryTree(bt, 5) == false);
	assert(getSizeOfBinaryTree(bt) == 0);

	destroyBinaryTree(bt);
}

void testBinarySearchTree07(CuTest* tc) {
	char* str;
	binary_tree* bt = initBinaryTree(integerValueOrderer, payloadDestructorNOP);
	int sum = 0;

	assert(getSizeOfBinaryTree(bt) == 0);

	sum = 0;
	ITERATE_THROUGH_BST(bt, n, int) {
		sum += n;
	}
	assert(sum == 0);

	destroyBinaryTree(bt);
}

void testBinarySearchTree08(CuTest* tc) {
	char* str;
	binary_tree* bt = initBinaryTree(integerValueOrderer, payloadDestructorNOP);
	int sum = 0;

	assert(getSizeOfBinaryTree(bt) == 0);
	addItemInBinaryTree(bt, 4);

	sum = 0;
	ITERATE_THROUGH_BST(bt, n, int) {
		sum += n;
	}
	critical("sum is %d", sum);
	assert(sum == 4);

	destroyBinaryTree(bt);
}

void testBinarySearchTree09(CuTest* tc) {
	char* str;
	binary_tree* bt = initBinaryTree(integerValueOrderer, payloadDestructorNOP);
	int sum = 0;

	assert(getSizeOfBinaryTree(bt) == 0);
	addItemInBinaryTree(bt, 4);
	addItemInBinaryTree(bt, 3);

	sum = 0;
	ITERATE_THROUGH_BST(bt, n, int) {
		info("adding %d to %d", n, sum);
		sum += n;
	}
	assert(sum == 7);

	destroyBinaryTree(bt);
}

void testBinarySearchTree10(CuTest* tc) {
	char* str;
	binary_tree* bt = initBinaryTree(integerValueOrderer, payloadDestructorNOP);
	int sum = 0;

	assert(getSizeOfBinaryTree(bt) == 0);
	addItemInBinaryTree(bt, 4);
	addItemInBinaryTree(bt, 5);

	sum = 0;
	ITERATE_THROUGH_BST(bt, n, int) {
		sum += n;
	}
	assert(sum == 9);

	destroyBinaryTree(bt);
}

void testBinarySearchTree11(CuTest* tc) {
	char* str;
	binary_tree* bt = initBinaryTree(integerValueOrderer, payloadDestructorNOP);
	int sum = 0;

	assert(getSizeOfBinaryTree(bt) == 0);
	addItemInBinaryTree(bt, 4);
	addItemInBinaryTree(bt, 3);
	addItemInBinaryTree(bt, 5);

	sum = 0;
	ITERATE_THROUGH_BST(bt, n, int) {
		sum += n;
	}
	assert(sum == 12);

	destroyBinaryTree(bt);
}

void testBinarySearchTree12(CuTest* tc) {
	char* str;
	binary_tree* bt = initBinaryTree(integerValueOrderer, payloadDestructorNOP);
	int sum = 0;

	assert(getSizeOfBinaryTree(bt) == 0);
	addItemInBinaryTree(bt, 4);
	addItemInBinaryTree(bt, 3);
	addItemInBinaryTree(bt, 5);
	addItemInBinaryTree(bt, 1);

	sum = 0;
	ITERATE_THROUGH_BST(bt, n, int) {
		sum += n;
	}
	assert(sum == 13);

	destroyBinaryTree(bt);
}

CuSuite* CuBinarySearchTreeSuite() {
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, testBinarySearchTree01);
	SUITE_ADD_TEST(suite, testBinarySearchTree02);
	SUITE_ADD_TEST(suite, testBinarySearchTree03);
	SUITE_ADD_TEST(suite, testBinarySearchTree04);
	SUITE_ADD_TEST(suite, testBinarySearchTree05);
	SUITE_ADD_TEST(suite, testBinarySearchTree06);
	SUITE_ADD_TEST(suite, testBinarySearchTree07);
	SUITE_ADD_TEST(suite, testBinarySearchTree08);
	SUITE_ADD_TEST(suite, testBinarySearchTree09);
	SUITE_ADD_TEST(suite, testBinarySearchTree10);
	SUITE_ADD_TEST(suite, testBinarySearchTree11);
	SUITE_ADD_TEST(suite, testBinarySearchTree12);

	return suite;
}


