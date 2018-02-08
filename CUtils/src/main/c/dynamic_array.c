/*
 * dynamic_array.c
 *
 *  Created on: Jul 12, 2017
 *      Author: koldar
 */


#include <stdlib.h>
#include <string.h>
#include "macros.h"
#include "dynamic_array.h"
#include "log.h"


typedef struct dynamic_1D_array {
	int size;
	int cellSize;
	char* array;
};

typedef struct dynamic_2D_matrix {
	int rows;
	int columns;
	dynamic_1D_array matrix;
};

#define POINTER_OF_CELL(da, i) &da->array[(i)*(da->cellSize)]

static setupDynamicArray(dynamic_1D_array* da, size_t cellSize, int cellsNumber);

dynamic_1D_array* _initDynamicArray(size_t cellSize, int cellsNumber) {
	dynamic_1D_array* retVal = malloc(sizeof(dynamic_1D_array));
	if (retVal == NULL) {
		ERROR_MALLOC();
	}

	setupDynamicArray(retVal, cellSize, cellsNumber);

	return retVal;
}

void destroyDynamicArray(dynamic_1D_array* da) {
	free(da->array);
	free(da);
}

void _setDynamicArrayCellTo(dynamic_1D_array* da, int cellNumber, void* value) {
#ifdef DEBUG
	if (cellNumber < 0 || cellNumber > da->size) {
		ERROR_NULL("index out of bounds");
	}
#endif
	memcpy(POINTER_OF_CELL(da, cellNumber), value, da->cellSize);
}

void* _getDynamicArrayCellValue(const dynamic_1D_array* da, int cellNumber) {
#ifdef DEBUG
	if (cellNumber < 0 || cellNumber > da->size) {
		ERROR_NULL("index out of bounds");
	}
#endif
	return POINTER_OF_CELL(da, cellNumber);
}

void _clearDynamicArrayTo(dynamic_1D_array* da, void* value) {
	for (int i=0; i<da->size; i++) {
		_setDynamicArrayCellTo(da, i, value);
	}
}

int getSizeOfDynamicArray(const dynamic_1D_array* da) {
	return da->size;
}

dynamic_1D_array* cloneDynamicArray(const dynamic_1D_array* original) {
	dynamic_1D_array* retVal = _initDynamicArray(original->cellSize, original->size);
	memcpy(retVal->array, original->array, original->cellSize * original->size);

	return retVal;
}

char* _getDirectPointerOnArray(const dynamic_1D_array* da) {
	return da->array;
}

dynamic_2D_matrix* _initDynamic2DMatrix(size_t cellSize, int rows, int columns) {
	dynamic_2D_matrix* retVal = malloc(sizeof(dynamic_2D_matrix));
	if (retVal == NULL) {
		ERROR_NULL("index out of bounds");
	}

	retVal->rows = rows;
	retVal->columns = columns;

	setupDynamicArray(&retVal->matrix, cellSize, rows * columns);
	return retVal;
}

void destroyDynamic2DMatrix(dynamic_2D_matrix* d2m) {
	free(d2m->matrix.array);
	free(d2m);
}

void _setDynamic2DMatrixCellTo(dynamic_2D_matrix* d2m, int row, int column, void* value) {
#ifdef DEBUG
	if (row < 0 || row >= d2m->rows) {
		ERROR_NULL("index out of bounds");
	}
	if (column < 0  || column >= d2m->columns) {
		ERROR_NULL("index out of bounds");
	}
#endif
	_setDynamicArrayCellTo(&d2m->matrix, row * d2m->columns + column, value);
}

void* _getDynamic2DMatrixCellValue(const dynamic_2D_matrix* d2m, int row, int column) {
#ifdef DEBUG
	if (row < 0 || row >= d2m->rows) {
		ERROR_NULL("index out of bounds");
	}
	if (column < 0  || column >= d2m->columns) {
		ERROR_NULL("index out of bounds");
	}
#endif
	return _getDynamicArrayCellValue(&d2m->matrix, row * d2m->columns + column);
}

void _clearDynamic2DMatrixTo(dynamic_2D_matrix* d2m, void* value) {
	_clearDynamicArrayTo(&d2m->matrix, value);
}

int getRowsOf2DMatrix(const dynamic_2D_matrix* d2m) {
	return d2m->rows;
}

int getColumnsOf2DMatrix(const dynamic_2D_matrix* d2m) {
	return d2m->columns;
}

int getSizeOf2DMatrix(const dynamic_2D_matrix* d2m) {
	return getSizeOfDynamicArray(&d2m->matrix);
}

dynamic_2D_matrix* cloneDynamic2DMatrix(const dynamic_2D_matrix* original) {
	dynamic_2D_matrix* retVal = _initDynamic2DMatrix(original->matrix.cellSize, original->rows, original->columns);
	memcpy(retVal->matrix.array, original->matrix.array, original->matrix.cellSize * original->matrix.size);

	return retVal;
}

char* _getDirectPointerOnRow(const dynamic_2D_matrix* d2m, int row) {
	return POINTER_OF_CELL((&d2m->matrix), row*d2m->columns);
}

static setupDynamicArray(dynamic_1D_array* da, size_t cellSize, int cellsNumber) {
	da->cellSize = cellSize;
	da->size = cellsNumber;
	da->array = malloc(sizeof(char) * cellSize * cellsNumber);
	if (da->array == NULL) {
		ERROR_MALLOC();
	}

	return da;
}
