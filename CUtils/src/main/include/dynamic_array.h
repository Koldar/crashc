/*
 * dynamic_array.h
 *
 *  Created on: Jul 12, 2017
 *      Author: koldar
 */

#ifndef DYNAMIC_ARRAY_H_
#define DYNAMIC_ARRAY_H_

#include <stdlib.h>
#include "macros.h"

typedef struct dynamic_1D_array dynamic_1D_array;
typedef struct dynamic_2D_matrix dynamic_2D_matrix;

dynamic_1D_array* _initDynamicArray(size_t cellSize, int cellsNumber);

void destroyDynamicArray(dynamic_1D_array* da);

void _setDynamicArrayCellTo(dynamic_1D_array* da, int cellNumber, void* value);

void* _getDynamicArrayCellValue(const dynamic_1D_array* da, int cellNumber);

void _clearDynamicArrayTo(dynamic_1D_array* da, void* value);

int getSizeOfDynamicArray(const dynamic_1D_array* da);

dynamic_1D_array* cloneDynamicArray(const dynamic_1D_array* original);

/**
 * Get the pointer of the element starting the array
 *
 * \note
 * Be careful: array index mismatching **will** alter other elements of the array!
 *
 * @param[in] da the dynamic array to operate on
 * @return the pointer where the first element of \c da is
 * 			You can treat such pointer as an array
 */
char* _getDirectPointerOnArray(const dynamic_1D_array* da);

/**
 * Initialize an array long \c cellsNumber elements, everyone of type \c type
 *
 * @param[in] type the name of the type that will be stored within the array. For example "int" or "list_cell"
 * @param[in] cellsNumber the number of elements inside this array
 * @return an instance of ::dynamic_1D_array
 */
#define initDynamicArray(type, cellsNumber) _initDynamicArray(sizeof(type), cellsNumber)
/**
 * Set the value of a cell inside a ::dynamic_1D_array
 *
 * @param[inout] da the array to alter
 * @param[in] cellNumber the index of the cell to edit
 * @param[in] type the type of the value inside \c value
 * @param[in] value the value to add inside the array. Values passed by **value**;
 */
#define setDynamicArrayCellTo(da, cellNumber, type, value) {\
	type UV(tmp) = (value); \
	_setDynamicArrayCellTo(da, cellNumber, (void*)(&UV(tmp))); \
}

/**
 * Like ::setDynamicArrayCellTo but requires you to put a **lvalue** in variable
 *
 * \pre
 * 	\li \c variable is a **l-value**;
 *
 * @parma[inout] da the array to alter
 * @param[in] cellNumber the index of the cell to edit
 * @param[in] type the type of the value inside \c value
 * @param[in] variable an **lvalue** containing the value to put inside the array
 */
#define setDynamicArrayCellToVariableValue(da, cellNumber, type, variable) _setDynamicArrayCellTo(da, cellNumber, &variable)

/**
 * Retrive a value within the dynamic array
 *
 * @param[in] da the dynamic array where we want to fetch the data from
 * @param[in] cellNumber the index of the cell to retrieve;
 * @param[in] type the type of the value to retrieve. For example "int" or "list_cell";
 */
#define getDynamicArrayCellValue(da, cellNumber, type) (*((type*)_getDynamicArrayCellValue(da, cellNumber)))

#define clearDynamicArrayTo(da, type, value) { \
		type UV(tmp) = (value); \
		_clearDynamicArrayTo(da, (void*)(&UV(tmp))); \
	}

#define getDirectPointerOnArray(da, type) ((type*)_getDirectPointerOnArray(da))

dynamic_2D_matrix* _initDynamic2DMatrix(size_t cellSize, int rows, int columns);

void destroyDynamic2DMatrix(dynamic_2D_matrix* d2m);

void _setDynamic2DMatrixCellTo(dynamic_2D_matrix* d2m, int row, int colum, void* value);

void* _getDynamic2DMatrixCellValue(const dynamic_2D_matrix* d2m, int row, int column);

void _clearDynamic2DMatrixTo(dynamic_2D_matrix* d2m, void* value);

int getRowsOf2DMatrix(const dynamic_2D_matrix* d2m);

int getColumnsOf2DMatrix(const dynamic_2D_matrix* d2m);

int getSizeOf2DMatrix(const dynamic_2D_matrix* d2m);

dynamic_2D_matrix* cloneDynamic2DMatrix(const dynamic_2D_matrix* original);

/**
 * Get the pointer of the element starting the given row in the matrix
 *
 * \note
 * Be careful: array index mismatching **will** alter other elements of the matrix!
 *
 * @param[in] d2m the matrix to operate on
 * @param[in] row the row we're looking for
 * @return the pointer where the first element of \c row is.
 * 			You can treat such pointer as an array to look inside the given row of the matrix
 */
char* _getDirectPointerOnRow(const dynamic_2D_matrix* d2m, int row);

#define initDynamic2DMatrix(type, rows, columns) _initDynamic2DMatrix(sizeof(type), rows, columns)
#define setDynamic2DMatrixCellTo(da, row, column, type, value) { \
	type UV(tmp) = (value); \
	_setDynamic2DMatrixCellTo(da, row, column, &UV(tmp)); \
}

#define getDynamic2DMatrixCellValue(da, row, column, type) (*((type*)_getDynamic2DMatrixCellValue(da, row, column)))
#define clearDynamic2DMatrixTo(da, type, value) { \
	type UV(tmp) = (value); \
	_clearDynamic2DMatrixTo(da, &UV(tmp)); \
}

#define getDirectPointerOnRow(da, row, type) ((type*)_getDirectPointerOnRow(da, row))

/**
 * Convenience macro to automatically iterate through a dynamic array
 *
 * An example of usage might be;
 * @code
 * 	ITERATE_THROUGH_DYNAMIC_ARRAY(arrayOfDoubles, i, v, double) {
 * 		printf("at index %d there is the double %2.3f\n", i, v);
 * 	}
 * @endcode
 */
#define ITERATE_THROUGH_DYNAMIC_ARRAY(_da, index, var, type) \
	dynamic_1D_array* UV(da) = (_da); \
	for ( \
			int index=0, UV(keepGoing)=1 \
			; \
			index<getSizeOfDynamicArray(UV(da)) \
			; \
			index++, UV(keepGoing)=1 \
	) \
		for (type var=getDynamicArrayCellValue(UV(da), index, type); UV(keepGoing) == 1; UV(keepGoing)=0)


#endif /* DYNAMIC_ARRAY_H_ */
