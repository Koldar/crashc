/*
 * \file dynamic_stack.h
 *
 * Represents a stack which, when filled, can resize itself to accomodate further elements.
 * Hence this stack keeps enlarging until there is no more memory available.
 *
 *  Created on: Oct 11, 2017
 *      Author: koldar
 */

#ifndef DYNAMIC_STACK_H_
#define DYNAMIC_STACK_H_

#include <stdbool.h>
#include "payload_functions.h"

typedef struct dynamic_stack dynamic_stack;

/**
 * Initialize dynamic stack
 *
 * @param[in] initialCapacity the initial capacity of the stack
 * @param[in] deltaSize when trying to add an item to a full stack, the stack is increased by this amount of cells
 * @param[in] pf functions used to manage the payload
 * @return
 * 	\li an instance of this stack;
 */
dynamic_stack* initDynamicStack(unsigned int initialCapacity, unsigned int deltaSize, payload_functions pf);

/**
 * Release the memory from a dynamic stack instance
 *
 * \note
 * 	\li the values pointed from this stack won't be removed from the memory.
 *
 * @param[in] ds the stack to release
 */
void destroyDynamicStack(dynamic_stack* ds);

/**
 * like ::destroyDynamicStack but it destroys the elements inside the stack from the memory as well
 *
 * @param[in] ds the stack to release
 */
void destroyDynamicStackWithElements(dynamic_stack* ds);

/**
 * Adds an item in top of the stack
 *
 * @param[inout] ds the stack where to push to
 * @param[in] element the item to add
 */
void pushItemToDynamicStack(dynamic_stack* ds, const void* element);

/**
 * Pop the top from the stack
 *
 * @param[inout] ds the stack where we need to pop from
 * @return
 * 	\li the item on the top;
 * 	\li NULL if the stack was actually empty;
 */
void* popItemFromDynamicStack(dynamic_stack* ds);

/**
 * Peek from the stack
 *
 * @param[in] ds the stack to peek from
 * @return
 * 	\li the item on the top of the stack;
 * 	\li NULL if the stack was actually empty;
 */
void* peekItemFromDynamicStack(const dynamic_stack* ds);

/**
 * Check if the stack is empty
 *
 * @param[in] ds the stack to analyze
 * @return true if the stack is completely empty
 */
bool isDynamicStackEmpty(const dynamic_stack* ds);

/**
 * Check if the stack is full
 *
 * @param[in] ds the stack to analyze
 * @return true if the stack is completely full
 */
bool isDynamicStackFull(const dynamic_stack* ds);

/**
 * Get the number of elements in the stack
 *
 * @param[in] ds the stack to analyze
 * @return the number of elements inside the stack
 */
unsigned int getSizeOfDynamicStack(const dynamic_stack* ds);

/**
 * Clear all the elements inside the stack.
 *
 * if the elements are pointers, the pointed structures won't be release from the memory **at all**!
 *
 * \post
 * 	\li stack is empty;
 *
 * @param[inout] ds the stack to clear
 */
void clearDynamicStack(dynamic_stack* ds);

/**
 * Iterate thorugh the stack without actually changing it
 *
 * The iteration goes from the top of the stack till the bottom
 *
 * @param[in] _stack the stack to analyze
 * @param[in] _payload a name of the variable containing the payload of the stack
 * @param[in] _payload_type the type of the _payload variable
 */
#define ITERATE_THROUGH_DYNAMIC_STACK(_stack, _payload, _payload_type) \
	dynamic_stack* UV(stack) = _stack; \
	bool UV(innerLoop); \
	for (int UV(i)=(getSizeOfDynamicStack(UV(stack)) - 1); (UV(innerLoop) = true, UV(i) >= 0); UV(i)--) \
		for (_payload_type _payload=(_payload_type)(getNthItemOfDynamicStack(UV(stack), UV(i))); UV(innerLoop); UV(innerLoop)=false) \

#endif /* DYNAMIC_STACK_H_ */
