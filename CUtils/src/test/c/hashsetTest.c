/*
 * hashsetTest.c
 *
 *  Created on: Dec 21, 2017
 *      Author: koldar
 */

#include <assert.h>
#include "CuTest.h"
#include "payload_functions.h"
#include "hash_set.h"
#include "log.h"

void testHashSet01(CuTest* tc) {
	pint_hash_set* intSet = initHashSet(getIntPayloadFunctions());
	destroyHashSetWithElements(intSet);
}

void testHashSet02(CuTest* tc) {
	pint_hash_set* intSet = initHashSet(getIntPayloadFunctions());

	assert(isHashSetEmpty(intSet));
	addItemInHashSet(intSet, 5);
	assert(!isHashSetEmpty(intSet));
	assert(getSizeOfHashSet(intSet) == 1);

	destroyHashSetWithElements(intSet);
}

void testHashSet03(CuTest* tc) {
	pint_hash_set* intSet = initHashSet(getIntPayloadFunctions());

	assert(isHashSetEmpty(intSet));
	addItemInHashSet(intSet, 5);
	addItemInHashSet(intSet, 5);
	assert(!isHashSetEmpty(intSet));
	assert(getSizeOfHashSet(intSet) == 1);

	destroyHashSetWithElements(intSet);
}

void testHashSet04(CuTest* tc) {
	pint_hash_set* intSet = initHashSet(getIntPayloadFunctions());

	assert(isHashSetEmpty(intSet));
	addItemInHashSet(intSet, 5);
	addItemInHashSet(intSet, 3);
	assert(!isHashSetEmpty(intSet));
	assert(getSizeOfHashSet(intSet) == 2);

	destroyHashSetWithElements(intSet);
}

void testHashSet05(CuTest* tc) {
	pint_hash_set* intSet = initHashSet(getIntPayloadFunctions());

	assert(isHashSetEmpty(intSet));

	addItemInHashSet(intSet, 5);
	addItemInHashSet(intSet, 3);
	removeItemInHashSet(intSet, 3);

	assert(!isHashSetEmpty(intSet));
	assert(getSizeOfHashSet(intSet) == 1);
	assert(containsItemInHashSet(intSet, 5));
	assert(!containsItemInHashSet(intSet, 3));

	destroyHashSetWithElements(intSet);
}

void testHashSet06(CuTest* tc) {
	pint_hash_set* intSet = initHashSet(getIntPayloadFunctions());
	int sum;

	assert(isHashSetEmpty(intSet));

	addItemInHashSet(intSet, 5);
	addItemInHashSet(intSet, 3);
	addItemInHashSet(intSet, 4);

	assert(!isHashSetEmpty(intSet));
	assert(getSizeOfHashSet(intSet) == 3);

	assert(containsItemInHashSet(intSet, 3));
	assert(containsItemInHashSet(intSet, 4));
	assert(containsItemInHashSet(intSet, 5));

	sum = 0;
	ITERATE_OVER_HASHSET(intSet, var, int) {
		sum += var;
	}
	assert(sum == 12);

	destroyHashSetWithElements(intSet);
}

void testHashSet07(CuTest* tc) {
	pint_hash_set* a = initHashSet(getIntPayloadFunctions());
	pint_hash_set* b = initHashSet(getIntPayloadFunctions());
	pint_hash_set* the_union;

	addItemInHashSet(a, 1);
	addItemInHashSet(a, 2);
	addItemInHashSet(a, 3);

	addItemInHashSet(b, 2);
	addItemInHashSet(b, 3);
	addItemInHashSet(b, 4);

	the_union = getUnionOfHashSets(a, b, false);

	assert(the_union != a);
	assert(the_union != b);
	assert(getSizeOfHashSet(a) == 3);
	assert(getSizeOfHashSet(b) == 3);

	assert(getSizeOfHashSet(the_union) == 4);
	assert(containsItemInHashSet(the_union, 1));
	assert(containsItemInHashSet(the_union, 2));
	assert(containsItemInHashSet(the_union, 3));
	assert(containsItemInHashSet(the_union, 4));

	destroyHashSetWithElements(a);
	destroyHashSetWithElements(b);
	destroyHashSetWithElements(the_union);
}

void testHashSet08(CuTest* tc) {
	pint_hash_set* a = initHashSet(getIntPayloadFunctions());
	pint_hash_set* b = initHashSet(getIntPayloadFunctions());
	pint_hash_set* the_intersection;

	addItemInHashSet(a, 1);
	addItemInHashSet(a, 2);
	addItemInHashSet(a, 3);

	addItemInHashSet(b, 2);
	addItemInHashSet(b, 3);
	addItemInHashSet(b, 4);

	bcritical("set1 is: %s", (getBufferStringOfHashSet(a, log_buffer), log_buffer));
	bcritical("set2 is: %s", (getBufferStringOfHashSet(b, log_buffer), log_buffer));
	the_intersection = getIntersectionOfHashSets(a, b, false);

	assert(the_intersection != a);
	assert(the_intersection != b);
	assert(getSizeOfHashSet(a) == 3);
	assert(getSizeOfHashSet(b) == 3);

	assert(getSizeOfHashSet(the_intersection) == 2);
	assert(containsItemInHashSet(the_intersection, 2));
	assert(containsItemInHashSet(the_intersection, 3));

	destroyHashSetWithElements(a);
	destroyHashSetWithElements(b);
	destroyHashSetWithElements(the_intersection);
}

void testHashSet09(CuTest* tc) {
	pint_hash_set* a = initHashSet(getIntPayloadFunctions());
	pint_hash_set* b = initHashSet(getIntPayloadFunctions());
	pint_hash_set* the_difference;

	addItemInHashSet(a, 1);
	addItemInHashSet(a, 2);
	addItemInHashSet(a, 3);

	addItemInHashSet(b, 2);
	addItemInHashSet(b, 3);
	addItemInHashSet(b, 4);

	the_difference = getDifferenceOfHashSets(a, b, false);

	assert(the_difference != a);
	assert(the_difference != b);
	assert(getSizeOfHashSet(a) == 3);
	assert(getSizeOfHashSet(b) == 3);

	assert(getSizeOfHashSet(the_difference) == 1);
	assert(containsItemInHashSet(the_difference, 1));

	destroyHashSetWithElements(a);
	destroyHashSetWithElements(b);
	destroyHashSetWithElements(the_difference);
}

void testHashSet10(CuTest* tc) {
	pint_hash_set* a = initHashSet(getIntPayloadFunctions());
	pint_hash_set* b = initHashSet(getIntPayloadFunctions());
	pint_hash_set* the_union;

	addItemInHashSet(a, 1);
	addItemInHashSet(a, 2);
	addItemInHashSet(a, 3);

	addItemInHashSet(b, 2);
	addItemInHashSet(b, 3);
	addItemInHashSet(b, 4);

	the_union = getUnionOfHashSets(a, b, true);

	assert(the_union == a);
	assert(the_union != b);
	assert(getSizeOfHashSet(a) == 4);
	assert(getSizeOfHashSet(b) == 3);

	assert(getSizeOfHashSet(the_union) == 4);
	assert(containsItemInHashSet(the_union, 1));
	assert(containsItemInHashSet(the_union, 2));
	assert(containsItemInHashSet(the_union, 3));
	assert(containsItemInHashSet(the_union, 4));

	destroyHashSetWithElements(a);
	destroyHashSetWithElements(b);
}

void testHashSet11(CuTest* tc) {
	pint_hash_set* a = initHashSet(getIntPayloadFunctions());
	pint_hash_set* b = initHashSet(getIntPayloadFunctions());
	pint_hash_set* the_intersection;

	addItemInHashSet(a, 1);
	addItemInHashSet(a, 2);
	addItemInHashSet(a, 3);

	addItemInHashSet(b, 2);
	addItemInHashSet(b, 3);
	addItemInHashSet(b, 4);

	the_intersection = getIntersectionOfHashSets(a, b, true);

	assert(the_intersection == a);
	assert(the_intersection != b);
	assert(getSizeOfHashSet(a) == 2);
	assert(getSizeOfHashSet(b) == 3);

	assert(getSizeOfHashSet(the_intersection) == 2);
	assert(containsItemInHashSet(the_intersection, 2));
	assert(containsItemInHashSet(the_intersection, 3));

	destroyHashSetWithElements(a);
	destroyHashSetWithElements(b);
}

void testHashSet12(CuTest* tc) {
	pint_hash_set* a = initHashSet(getIntPayloadFunctions());
	pint_hash_set* b = initHashSet(getIntPayloadFunctions());
	pint_hash_set* the_difference;

	addItemInHashSet(a, 1);
	addItemInHashSet(a, 2);
	addItemInHashSet(a, 3);

	addItemInHashSet(b, 2);
	addItemInHashSet(b, 3);
	addItemInHashSet(b, 4);

	the_difference = getDifferenceOfHashSets(a, b, true);

	assert(the_difference == a);
	assert(the_difference != b);
	assert(getSizeOfHashSet(a) == 1);
	assert(getSizeOfHashSet(b) == 3);

	assert(getSizeOfHashSet(the_difference) == 1);
	assert(containsItemInHashSet(the_difference, 1));

	destroyHashSetWithElements(a);
	destroyHashSetWithElements(b);
}

CuSuite* CuHashSetSuite() {
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, testHashSet01);
	SUITE_ADD_TEST(suite, testHashSet02);
	SUITE_ADD_TEST(suite, testHashSet03);
	SUITE_ADD_TEST(suite, testHashSet04);
	SUITE_ADD_TEST(suite, testHashSet05);
	SUITE_ADD_TEST(suite, testHashSet06);
	SUITE_ADD_TEST(suite, testHashSet07);
	SUITE_ADD_TEST(suite, testHashSet08);
	SUITE_ADD_TEST(suite, testHashSet09);
	SUITE_ADD_TEST(suite, testHashSet10);
	SUITE_ADD_TEST(suite, testHashSet11);
	SUITE_ADD_TEST(suite, testHashSet12);

	return suite;
}
