/*
 * stack.h
 *
 *  Created on: Mar 13, 2017
 *      Author: koldar
 */

#ifndef STACK_H_
#define STACK_H_

#include <stdbool.h>
#include "macros.h"

/**
 * A data structure representing a stack
 */
typedef struct Stack {
	///the length of the array ::Stack::elements
	int maxSize;
	///the number of elements inside ::Stack::elements
	int size;
	///an array representing the stack itself
	void** elements;
} Stack;

/**
 * Initialize a new stack
 *
 * \attention
 * Be sure to free it from the heap by calling ::destroyStack when it is not useful anymore
 *
 * @param[in] maxSize the size of the stack
 */
Stack* initStack(int maxSize);

/**
 * Destory from the heap a stack created with ::initStack
 *
 * @param[in] stack the stack to remove from the memory
 */
void destroyStack(Stack* stack);

/**
 * @param[in] stack the structure to check
 * @return the number of elements inside the stack
 */
int getStackSize(Stack* stack);

/**
 * @param[in] s the stack to analyze
 * @return the maximum capability of the stack
 */
int getStackMaxSize(const Stack* s);

/**
 * Stacks empty can perform no \b pop operation
 *
 * @param[in] stack the structure to check
 * @return
 * 	\li true if the stack has no elements inside it;
 * 	\li false otherwise
 */
bool isStackEmpty(Stack* stack);

/**
 * Stacks full can perform no \b push operation
 *
 * @param[in] stack the structure to check
 * @return
 * 	\li true if the stack is completely full
 * 	\li false otherwise
 */
bool isStackFull(Stack* stack);

/**
 * Pops an element from the stack
 *
 * The stack will decrease its size after this operation
 *
 * @param[in] stack the structure to check
 * @return
 * 	\li the element on the top of the stack;
 * 	\li NULL otherwise
 */
void* popFromStack(Stack* stack);

/**
 * Push an elemento into the stack
 *
 * The stack will increase its size after this operation
 *
 * \attention
 * 	The function will crash if you try to push an element to a full stack
 *
 * @param[in] stack the structure to check
 * @param[in] data the data to push to the stack
 */
void pushToStack(Stack* stack, void* data);

/**
 * like ::popFromStack, but it won't alter the stack itself
 */
void* peekFromStack(Stack* stack);

/**
 * Empty the stack
 *
 * \note
 * If there is data on the stack, it not \b free automatically. It is simple removed. Be sure
 * there are other references in the program to avoid memory leaks
 *
 * @param[in] stack the stack to clear
 */
void clearStack(Stack* stack);

/**
 * Iterate thorugh the stack without actually changing it
 *
 * The iteration goes from the top of the stack till the bottom
 *
 * @param[in] _stack the stack to analyze
 * @param[in] _payload a name of the variable containing the payload of the stack
 * @param[in] _payload_type the type of the _payload variable
 */
#define ITERATE_THROUGH_STACK(_stack, _payload, _payload_type) \
	Stack* UV(stack) = _stack; \
	bool UV(innerLoop); \
	for (int UV(i)=(stack->size - 1); (UV(innerLoop) = true, UV(i) >= 0); UV(i)--) \
		for (_payload_type _payload=(_payload_type)(stack->elements[UV(i)]); UV(innerLoop); UV(innerLoop)=false) \


#endif /* STACK_H_ */
