/*
 * stack.c
 *
 *  Created on: Mar 13, 2017
 *      Author: koldar
 */

#include "stack.h"
#include "utility.h"
#include <stdlib.h>

Stack* initStack(int maxSize) {
	Stack* retVal = malloc(sizeof(Stack));
	if (retVal == NULL) {
		ERROR_MALLOC();
	}

	retVal->size = 0;
	retVal->maxSize = maxSize;
	retVal->elements = calloc(retVal->maxSize, sizeof(void*));
	if (retVal->elements == NULL) {
		ERROR_MALLOC();
	}

	return retVal;
}

void destroyStack(Stack* stack) {
	free(stack->elements);
	free(stack);
}

int getStackSize(Stack* stack) {
	return stack->size;
}

int getStackMaxSize(const Stack* s) {
	return s->maxSize;
}

bool isStackEmpty(Stack* stack) {
	return stack->size == 0;
}

bool isStackFull(Stack* stack) {
	return stack->size == stack->maxSize;
}

void* popFromStack(Stack* stack) {
	if (isStackEmpty(stack)) {
		return NULL;
	}
	stack->size--;
	return stack->elements[stack->size];
}

void pushToStack(Stack* stack, void* data) {
	if (isStackFull(stack)) {
		ERROR_OBJECT_IS_FULL("stack", stack->size);
	}
	stack->elements[stack->size] = data;
	stack->size++;
}

void* peekFromStack(Stack* stack) {
	if (isStackEmpty(stack)) {
		return NULL;
	}
	return stack->elements[stack->size - 1];
}

void clearStack(Stack* stack) {
	stack->size = 0;
}
