/*
 * htTest.c
 *
 *  Created on: May 18, 2017
 *      Author: koldar
 */


#include "CuTest.h"
#include "hashtable.h"
#include <string.h>
#include <assert.h>
#include "macros.h"

///test initHT
void testHT01(CuSuite* suite) {
	HT* ht = initHT();
	destroyHT(&ht);
}

///test addItemInHTWithKey
void testHT02(CuSuite* suite) {
	HT* ht = initHT();

	assert(getSizeOfHT(&ht) == 0);
	assert(isHTEmpty(&ht) == true);
	int a = 3;
	addItemInHTWithKey(&ht, 5, &a);

	assert(getSizeOfHT(&ht) == 1);
	assert(isHTEmpty(&ht) == false);

	deleteItemInHT(&ht, 5);

	assert(getSizeOfHT(&ht) == 0);
	assert(isHTEmpty(&ht) == true);

	destroyHT(&ht);
}

///test ITERATE_ON_HT
void testHT03(CuSuite* suite) {
	HT* ht = initHT();

	int a = 5;
	int b = 6;
	int c = 7;
	int d = 10;

	assert(getSizeOfHT(&ht) == 0);
	assert(isHTEmpty(&ht) == true);

	addItemInHTWithKey(&ht, 5, &a);
	addItemInHTWithKey(&ht, 6, &b);
	addItemInHTWithKey(&ht, 7, &c);
	addItemInHTWithKey(&ht, 10, &d);

	assert(getSizeOfHT(&ht) == 4);
	assert(isHTEmpty(&ht) == false);

	int sum = 0;
	ITERATE_ON_HT(&ht, pair) {
		sum += *((int*)(pair->data));
	}

	assert(sum == 28);

	destroyHT(&ht);
}

///test getItemInHT
void testHT04(CuSuite* suite) {
	HT* ht = initHT();

	int a = 5;
	int b = 6;
	int c = 7;
	int d = 10;

	assert(getSizeOfHT(&ht) == 0);
	assert(isHTEmpty(&ht) == true);

	addItemInHTWithKey(&ht, 5, &a);
	addItemInHTWithKey(&ht, 6, &b);
	addItemInHTWithKey(&ht, 7, &c);
	addItemInHTWithKey(&ht, 10, &d);

	assert (getItemInHT(&ht, 11) == NULL);
	assert (*(int*)getItemInHT(&ht, 7) == 7);
	assert (*(int*)getItemInHT(&ht, 5) == 5);
	assert (*(int*)getItemInHT(&ht, 10) == 10);

	destroyHT(&ht);
}

///test ITERATE_VALUES_ON_HT
void testHT05(CuSuite* suite) {
	HT* ht = initHT();

	int a = 5;
	int b = 6;
	int c = 7;
	int d = 10;

	assert(getSizeOfHT(&ht) == 0);
	assert(isHTEmpty(&ht) == true);

	addItemInHTWithKey(&ht, 5, &a);
	addItemInHTWithKey(&ht, 6, &b);
	addItemInHTWithKey(&ht, 7, &c);
	addItemInHTWithKey(&ht, 10, &d);

	assert(getSizeOfHT(&ht) == 4);
	assert(isHTEmpty(&ht) == false);

	int sum = 0;
	ITERATE_VALUES_ON_HT(&ht, value, int) {
		sum += *value;
	}

	assert(sum == 28);

	destroyHT(&ht);
}

CuSuite* CuHTSuite() {
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, testHT01);
	SUITE_ADD_TEST(suite, testHT02);
	SUITE_ADD_TEST(suite, testHT03);
	SUITE_ADD_TEST(suite, testHT04);
	SUITE_ADD_TEST(suite, testHT05);



	return suite;
}


