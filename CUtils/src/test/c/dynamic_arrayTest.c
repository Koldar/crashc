/*
 * dynamic_arrayTest.c
 *
 *  Created on: Jul 12, 2017
 *      Author: koldar
 */

#include "dynamic_array.h"
#include "CuTest.h"
#include <assert.h>

///test dynamic array allocation and deallocation
void testDynamicArray01(CuTest* tc) {
	dynamic_1D_array* da = initDynamicArray(int, 10);

	destroyDynamicArray(da);
}

///test setting and getting a cell
void testDynamicArray02(CuTest* tc) {
	dynamic_1D_array* da = initDynamicArray(int, 10);

	setDynamicArrayCellTo(da, 0, int, 5);
	assert(getDynamicArrayCellValue(da, 0, int) == 5);
	setDynamicArrayCellTo(da, 3, int, 5);
	assert(getDynamicArrayCellValue(da, 3, int) == 5);
	setDynamicArrayCellTo(da, 3, int, 4);
	assert(getDynamicArrayCellValue(da, 3, int) == 4);

	destroyDynamicArray(da);
}

///test clearing a dynamic array
void testDynamicArray03(CuTest* tc) {
	dynamic_1D_array* da = initDynamicArray(int, 10);

	setDynamicArrayCellTo(da, 0, int, 5);
	assert(getDynamicArrayCellValue(da, 0, int) == 5);
	setDynamicArrayCellTo(da, 3, int, 5);
	assert(getDynamicArrayCellValue(da, 3, int) == 5);
	setDynamicArrayCellTo(da, 3, int, 4);
	assert(getDynamicArrayCellValue(da, 3, int) == 4);

	assert(getSizeOfDynamicArray(da) == 10);

	clearDynamicArrayTo(da, int, 10);
	for (int i=0; i<getSizeOfDynamicArray(da); i++) {
		assert(getDynamicArrayCellValue(da, i, int) == 10);
	}

	destroyDynamicArray(da);
}

///test setting and getting the first and the last cell of the dynamic array
void testDynamicArray04(CuTest* tc) {
	dynamic_1D_array* da = initDynamicArray(int, 10);

	setDynamicArrayCellTo(da, 0, int, 5);
	assert(getDynamicArrayCellValue(da, 0, int) == 5);
	setDynamicArrayCellTo(da, 0, int, 4);
	assert(getDynamicArrayCellValue(da, 0, int) == 4);

	setDynamicArrayCellTo(da, 9, int, 5);
	assert(getDynamicArrayCellValue(da, 9, int) == 5);
	setDynamicArrayCellTo(da, 9, int, 4);
	assert(getDynamicArrayCellValue(da, 9, int) == 4);

	destroyDynamicArray(da);
}

///test cloning
void testDynamicArray05(CuTest* tc) {
	dynamic_1D_array* da = initDynamicArray(int, 4);

	setDynamicArrayCellTo(da, 0, int, 5);
	setDynamicArrayCellTo(da, 1, int, 6);
	setDynamicArrayCellTo(da, 2, int, 7);
	setDynamicArrayCellTo(da, 3, int, 8);

	dynamic_1D_array* clone = cloneDynamicArray(da);

	assert(getSizeOfDynamicArray(da) == 4);
	assert(da != clone);

	assert(getDynamicArrayCellValue(clone, 0, int) == 5);
	assert(getDynamicArrayCellValue(clone, 1, int) == 6);
	assert(getDynamicArrayCellValue(clone, 2, int) == 7);
	assert(getDynamicArrayCellValue(clone, 3, int) == 8);

	destroyDynamicArray(da);
	destroyDynamicArray(clone);
}

///test dynamic 2d matrix allocation and deallocation
void testDynamic2DMatrix01(CuTest* tc) {
	dynamic_2D_matrix* d2m = initDynamic2DMatrix(int, 5, 3);

	destroyDynamic2DMatrix(d2m);
}

///test setting and getting a cell
void testDynamic2DMatrix02(CuTest* tc) {
	dynamic_2D_matrix* d2m = initDynamic2DMatrix(int, 5, 3);

	setDynamic2DMatrixCellTo(d2m, 3, 1, int, 5);
	assert(getDynamic2DMatrixCellValue(d2m, 3, 1, int) == 5);
	setDynamic2DMatrixCellTo(d2m, 3, 2, int, 5);
	assert(getDynamic2DMatrixCellValue(d2m, 3, 2, int) == 5);
	setDynamic2DMatrixCellTo(d2m, 3, 2, int, 4);
	assert(getDynamic2DMatrixCellValue(d2m, 3, 2, int) == 4);

	destroyDynamic2DMatrix(d2m);
}

///test clearing a dynamic array
void testDynamic2DMatrix03(CuTest* tc) {
	dynamic_2D_matrix* d2m = initDynamic2DMatrix(int, 5, 3);

	setDynamic2DMatrixCellTo(d2m, 3, 1, int, 5);
	assert(getDynamic2DMatrixCellValue(d2m, 3, 1, int) == 5);
	setDynamic2DMatrixCellTo(d2m, 3, 2, int, 5);
	assert(getDynamic2DMatrixCellValue(d2m, 3, 2, int) == 5);
	setDynamic2DMatrixCellTo(d2m, 3, 2, int, 4);
	assert(getDynamic2DMatrixCellValue(d2m, 3, 2, int) == 4);

	assert(getRowsOf2DMatrix(d2m) == 5);
	assert(getColumnsOf2DMatrix(d2m) == 3);
	assert(getSizeOf2DMatrix(d2m) == 15);

	clearDynamic2DMatrixTo(d2m, int, 10);
	for (int i=0; i<getRowsOf2DMatrix(d2m); i++) {
		for (int j=0; j<getColumnsOf2DMatrix(d2m); j++) {
			assert(getDynamic2DMatrixCellValue(d2m, i, j, int) == 10);
		}
	}

	destroyDynamic2DMatrix(d2m);
}

///test setting and getting the first and the last cell of the dynamic array
void testDynamic2DMatrix04(CuTest* tc) {
	dynamic_2D_matrix* d2m = initDynamic2DMatrix(int, 5, 3);

	setDynamic2DMatrixCellTo(d2m, 0, 0, int, 5);
	assert(getDynamic2DMatrixCellValue(d2m, 0, 0, int) == 5);
	setDynamic2DMatrixCellTo(d2m, 0, 0, int, 4);
	assert(getDynamic2DMatrixCellValue(d2m, 0, 0, int) == 4);

	setDynamic2DMatrixCellTo(d2m, 4, 2, int, 5);
	assert(getDynamic2DMatrixCellValue(d2m, 4, 2, int) == 5);
	setDynamic2DMatrixCellTo(d2m, 4, 2, int, 4);
	assert(getDynamic2DMatrixCellValue(d2m, 4, 2, int) == 4);

	destroyDynamic2DMatrix(d2m);
}


void testDynamic2DMatrix05(CuTest* tc) {
	dynamic_2D_matrix* d2m = initDynamic2DMatrix(int, 3, 3);

	for (int i=0; i<3; i++) {
		for (int j=0; j<3; j++) {
			setDynamic2DMatrixCellTo(d2m, i, j, int, i+j);
		}
	}

	assert(getDynamic2DMatrixCellValue(d2m, 0, 0, int) == 0);
	assert(getDynamic2DMatrixCellValue(d2m, 0, 1, int) == 1);
	assert(getDynamic2DMatrixCellValue(d2m, 0, 2, int) == 2);
	assert(getDynamic2DMatrixCellValue(d2m, 1, 0, int) == 1);
	assert(getDynamic2DMatrixCellValue(d2m, 1, 1, int) == 2);
	assert(getDynamic2DMatrixCellValue(d2m, 1, 2, int) == 3);
	assert(getDynamic2DMatrixCellValue(d2m, 2, 0, int) == 2);
	assert(getDynamic2DMatrixCellValue(d2m, 2, 1, int) == 3);
	assert(getDynamic2DMatrixCellValue(d2m, 2, 2, int) == 4);

	int* row1 = getDirectPointerOnRow(d2m, 1, int);

	row1[0] = 5;
	row1[1] = 5;
	row1[2] = 5;

	assert(getDynamic2DMatrixCellValue(d2m, 0, 0, int) == 0);
	assert(getDynamic2DMatrixCellValue(d2m, 0, 1, int) == 1);
	assert(getDynamic2DMatrixCellValue(d2m, 0, 2, int) == 2);
	assert(getDynamic2DMatrixCellValue(d2m, 1, 0, int) == 5);
	assert(getDynamic2DMatrixCellValue(d2m, 1, 1, int) == 5);
	assert(getDynamic2DMatrixCellValue(d2m, 1, 2, int) == 5);
	assert(getDynamic2DMatrixCellValue(d2m, 2, 0, int) == 2);
	assert(getDynamic2DMatrixCellValue(d2m, 2, 1, int) == 3);
	assert(getDynamic2DMatrixCellValue(d2m, 2, 2, int) == 4);

	destroyDynamic2DMatrix(d2m);
}

///test cloning
void testDynamic2DMatrix06(CuTest* tc) {
	dynamic_2D_matrix* da = initDynamic2DMatrix(int, 3, 2);

	setDynamic2DMatrixCellTo(da, 0, 0, int, 5);
	setDynamic2DMatrixCellTo(da, 0, 1, int, 6);
	setDynamic2DMatrixCellTo(da, 1, 0, int, 7);
	setDynamic2DMatrixCellTo(da, 1, 1, int, 8);
	setDynamic2DMatrixCellTo(da, 2, 0, int, 9);
	setDynamic2DMatrixCellTo(da, 2, 1, int, 10);

	dynamic_2D_matrix* clone = cloneDynamic2DMatrix(da);

	assert(getSizeOf2DMatrix(clone) == 6);
	assert(da != clone);

	assert(getDynamic2DMatrixCellValue(clone, 0, 0, int) == 5);
	assert(getDynamic2DMatrixCellValue(clone, 0, 1, int) == 6);
	assert(getDynamic2DMatrixCellValue(clone, 1, 0, int) == 7);
	assert(getDynamic2DMatrixCellValue(clone, 1, 1, int) == 8);
	assert(getDynamic2DMatrixCellValue(clone, 2, 0, int) == 9);
	assert(getDynamic2DMatrixCellValue(clone, 2, 1, int) == 10);

	destroyDynamic2DMatrix(da);
	destroyDynamic2DMatrix(clone);
}

CuSuite* CuDynamicArraySuite() {
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, testDynamicArray01);
	SUITE_ADD_TEST(suite, testDynamicArray02);
	SUITE_ADD_TEST(suite, testDynamicArray03);
	SUITE_ADD_TEST(suite, testDynamicArray04);
	SUITE_ADD_TEST(suite, testDynamicArray05);

	SUITE_ADD_TEST(suite, testDynamic2DMatrix01);
	SUITE_ADD_TEST(suite, testDynamic2DMatrix02);
	SUITE_ADD_TEST(suite, testDynamic2DMatrix03);
	SUITE_ADD_TEST(suite, testDynamic2DMatrix04);
	SUITE_ADD_TEST(suite, testDynamic2DMatrix05);
	SUITE_ADD_TEST(suite, testDynamic2DMatrix06);


	return suite;
}
