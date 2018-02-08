/*
 * dynamic_stack.c
 *
 *  Created on: Oct 11, 2017
 *      Author: koldar
 */

#include "dynamic_stack.h"
#include <stdlib.h>
#include "macros.h"

typedef struct dynamic_stack {
	/**
	 * The number of elements inside the stack
	 */
	unsigned int size;
	/**
	 * the maximum elements ::dynamic_stack::stack can contain (at the moment)
	 */
	unsigned int capacity;
	/**
	 * The number of cells ::dynamic_stack::stack will be enlarge of whenever it's full.
	 * Must be \f$ size > 0 \f$.
	 */
	unsigned int delta;
	/**
	 * functions used to manage abstractly the values inside this container
	 */
	payload_functions pf;
	/**
	 * the stack itself. Can be realloc'd
	 * It points to an array of `void*` located somewhere in the memory
	 */
	void** stack;
};

static void reallocStack(dynamic_stack* ds);

dynamic_stack* initDynamicStack(unsigned int initialCapacity, unsigned int deltaSize, payload_functions pf) {
	dynamic_stack* retVal = malloc(sizeof(dynamic_stack));
	if (retVal == NULL) {
		ERROR_MALLOC();
	}

	retVal->size = 0;
	retVal->capacity = initialCapacity;
	retVal->delta = deltaSize;
	retVal->pf = pf;

	retVal->stack = calloc(retVal->capacity, sizeof(void*));
	if (retVal->stack == NULL) {
		ERROR_MALLOC();
	}

	return retVal;
}

void destroyDynamicStack(dynamic_stack* ds) {
	free(ds->stack);
	free(ds);
}

void destroyDynamicStackWithElements(dynamic_stack* ds) {
	for (int i=0; i<ds->size; i++) {
		ds->pf.destroy(ds->stack[i]);
	}
	destroyDynamicStack(ds);
}

void pushItemToDynamicStack(dynamic_stack* ds, const void* element) {
	if (isDynamicStackFull(ds)) {
		reallocStack(ds);
	}
	ds->stack[ds->size] = element;
	ds->size++;
}

void* popItemFromDynamicStack(dynamic_stack* ds) {
	if (isDynamicStackEmpty(ds)) {
		return NULL;
	}

	void* retVal = ds->stack[ds->size-1];
	ds->size--;
	return retVal;
}

void* peekItemFromDynamicStack(const dynamic_stack* ds) {
	if (isDynamicStackEmpty(ds)) {
		return NULL;
	}

	return ds->stack[ds->size-1];
}

bool isDynamicStackEmpty(const dynamic_stack* ds) {
	return ds->size == 0;
}

bool isDynamicStackFull(const dynamic_stack* ds) {
	return ds->size == ds->capacity;
}

unsigned int getSizeOfDynamicStack(const dynamic_stack* ds) {
	return ds->size;
}

void clearDynamicStack(dynamic_stack* ds) {
	ds->size = 0;
}

void* getNthItemOfDynamicStack(const dynamic_stack* ds, int i) {
	return ds->stack[i];
}

/**
 * resize memory allocated to the actual stack
 *
 * \note
 * No changes are performed to the values inside the stack. So, if there is a pointer inside
 * the stack, that pointer won't changed. Of course, the pointer to the pointer will change.
 *
 * @param[inout] ds the dynamic stack whose capacity we need to increase
 */
static void reallocStack(dynamic_stack* ds) {
	//during this operation only ds->stack address changes. The values inside the stack won't be altered at all

	void** newPtr = realloc(ds->stack, sizeof(void*) * (ds->capacity + ds->delta));
	if (newPtr == NULL) {
		ERROR_MALLOC();
	}
	ds->stack = newPtr;
	ds->capacity += ds->delta;
}
