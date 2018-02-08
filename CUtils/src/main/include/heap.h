/*
 * \file heap.h
 *
 * Represents an heap implementation
 *
 * You can retrieve information on how heaps work <a href="https://www.cs.cmu.edu/~adamchik/15-121/lectures/Binary%20Heaps/heaps.html">here</a>.
 * The implementation is actually a min-heap implementation. However, by setting ::heap::relationship function correctly, one can easily transform
 * it into a max-heap.
 *
 *  Created on: Jun 2, 2017
 *      Author: koldar
 */

#ifndef HEAP_H_
#define HEAP_H_

#include <stdbool.h>
#include "macros.h"
#include "typedefs.h"

typedef struct heap heap;

/**
 * An heap storing integers
 */
typedef heap int_heap;

/**
 * Initialize the heap
 *
 * @param[in] maxSize the maximum number of elements that can be added inside the heap
 * @param[in] o a function used to compare the values within the heap
 * @return the heap requested
 */
heap* initHeap(int maxSize, destructor d, orderer o);

/**
 * Destroy an heap
 *
 * \note
 * this function **won't** free from the memory the values within the heap.
 * It will just release from memory the space allocated for the heap itself
 *
 * @param[in] h the heap to release from memory
 */
void destroyHeap(heap* h);

/**
 * Like ::destroyHeap but it will release from memory the data within the heap as well
 *
 * @param[in] h the heap to release from memory
 */
void destroyHeapWithElements(heap* h);

/**
 * Clear all the data inside the heap
 *
 * This **won't** free any memory allocated to the values inside the heap. It will just remove
 * them from the heap itself
 *
 * @param[in] h the heap involved
 */
void clearHeap(heap* h);

/**
 * Adds an item inside the heap
 *
 * If the item is already present inside the heap, the function does nothing
 *
 * \attention
 * The function will do nothing if the heap is full
 *
 * @param[in] h the heap to analyze
 * @param[in] item the element to add in the heap
 * @return
 * 	\li true if the item has been added in the heap;
 * 	\li false if the heap is full
 */
bool insertItemInHeap(heap* h, void* item);

/**
 * @param[in] h the heap involved
 * @return
 * 	\li true if the heap contains no elements
 */
bool isHeapEmpty(const heap* h);

/**
 * @param[in] h the heap involved
 * @return
 * 	\li true if the heap can't contain any more elements
 */
bool isHeapFull(const heap* h);

void* peekMinItemInHeap(const heap* h);

/**
 * Removes the minimum value in the heap.
 *
 * This operation takes \f$ O(n log_2 {n}) \f$ time
 *
 * @param[in] h the heap where we want to remove the item from
 * @return
 * 	\li the item with the least value in the heap;
 * 	\li NULL if the heap is empty;
 */
void* removeMinItemInHeap(heap* h);

/**
 * @param[in] h the heap to analyze
 * @return the number of elements inside the heap
 */
int getSizeOfHeap(const heap* h);

/**
 * @param[in] h the heap to analyze
 * @return the maximum number of elements the heap can contain
 */
int getMaxSizeOfHeap(const heap* h);

/**
 * Check if the heap contains an object
 *
 * @param[in] h the heap to analyze
 * @param[in] item the item to look for
 * @return
 * 	\li true if the heap contains such object;
 * 	\li false otherwise;
 */
bool containsItemInHeap(const heap* h, void* item);

/**
 * Get the Nth item in the heap
 *
 * \note
 * Use this function only if you know what you'redoing since this operation shouldn't be supported by an heap
 *
 * @param[in] h the heap to handle
 * @parma[in] i the position of the payload to return
 * @return the paylaod requested or NULL if it doesn't exist;
 */
void* getNthItemInHeap(const heap* h, int i);

void setBufferStringFunctionInHeap(heap* h, buffer_string bf);

int getBufferStringOfHeap(const heap* h, char* buffer);

void setClonerFunctionInHeap(heap* h, cloner c);

/**
 * Clone the given heap
 *
 * \note
 * the function uses the field ::heap::c to operate.
 * You need to setup it up via ::setClonerFunctionInHeap
 *
 * @param[in] h the heap to clone
 * @return the cloned instance
 */
heap* cloneHeap(const heap* h);

/**
 * Move all the elements inside an heap into another one
 *
 * After this operation, \c src will be empty;
 *
 * @param[inout] dst the heap that will receive all the elements of \c src;
 * @param[inout] dst the heap that will offer all its elements to \c dst;
 */
void moveHeapElements(heap* dst, heap* src);

/**
 * Used to iterate through the values of an heap
 *
 * \note
 * The values are not returned in any order. This iterastion doesn't support removal.
 *
 * Use this utility only for reading the heap content. Here's an example of usage:
 *
 * ```
 * ITERATE_THROUGH_HEAP(h, number, int) {
 * 	printf("the heap contains the number %d\n", number);
 * }
 * ```
 *
 * @param[in] h the heap to handle
 * @param[in] payload the variable name that will contain heap values. Its value changes each time
 * @param[in] payload_type the type of the variable \c payload. **No pointer will be added to this type**! It's up to you to manually add them!
 */
#define ITERATE_THROUGH_HEAP(h, payload, payload_type) \
	int UV(i) = 0; \
	payload_type payload = NULL; \
	if (getSizeOfHeap(h) > 0) { \
		payload = (payload_type) getNthItemInHeap(h, UV(i) + 1); \
	} \
	for ( \
			int UV(i)=0 \
			; \
			UV(i)<getSizeOfHeap(h) \
			; \
			UV(i)++, \
			payload = UV(i)<getSizeOfHeap(h) ? getNthItemInHeap(h, UV(i) + 1) : NULL \
		)



#endif /* HEAP_H_ */
