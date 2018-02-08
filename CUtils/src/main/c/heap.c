/*
 * heap.c
 *
 *  Created on: Jun 2, 2017
 *      Author: koldar
 */

#include "heap.h"
#include "macros.h"
#include <stdlib.h>
#include <math.h>
#include "log.h"
#include "defaultFunctions.h"

/**
 * Represents the id to return if the given cell doesn't have the requested cell
 * (ie root can't have parent node or leaf node can't have right child).
 */
#define H_NOID -1

typedef struct heap {
	/**
	 * The maximum number of values that can be added in the heap
	 */
	int maxSize;
	/**
	 * Represents the index fo the first leaf node in the heap.
	 *
	 * leaves are all at the end of the heap. So knowing the id of the first leaf
	 * means you can detect whether a node is a leaf or not very quickly by simply comparing
	 * its id with this variable
	 */
	int firstLeafNodeIndex;
	/**
	 * The number of elements inside this data structure
	 */
	int size;
	/**
	 * the function determines when a value of the heap should be a parent or a child of another one
	 */
	orderer relationship;
	/**
	 * The function to call when we want to release from memory an item inside the heap
	 */
	destructor d;
	/**
	 * the function used to produce string representation of this heap
	 */
	buffer_string bf;
	/**
	 * function used to clone the payload of the heap during cloning operation
	 */
	cloner c;
	/**
	 * Represents the index of a cell in ::heap::elements that should be filled with a new value next
	 */
	int nextCellAvailable;
	/**
	 * An array of elements
	 *
	 * The array is ::heap::maxSize long
	 */
	void** elements;
};

static bool isIndexRoot(const heap* h, int index);
static bool isLeafNode(const heap* h, int index);
static int getParentCell(const heap* h, int index);
static int getNewNextCellAvailable(const heap* h);
static int getLeftChild(const heap* h, int index);
static int getRightChild(const heap* h, int index);
static void percolateUp(heap* h, int index);
static void percolateDown(heap* h, int parentIndex);
static bool containsItemInHeapRecursive(const heap* h, int currentId, void* lookedFor);

heap* initHeap(int maxSize, destructor d, orderer o) {
	heap* retVal = malloc(sizeof(heap));
	if (retVal == NULL) {
		ERROR_MALLOC();
	}

	retVal->maxSize = maxSize;
	retVal->firstLeafNodeIndex = pow(2, floor(log2(retVal->maxSize)));
	retVal->size = 0;
	retVal->relationship = o;
	retVal->d = d;
	retVal->bf = defaultBufferString;
	retVal->nextCellAvailable = 1;
	//for implementation convenience, this array content start from 1, not from 0.
	//this because if the root has index 1, every left child has id equal to 2*parentNode while
	//every right node has id equal to 2*parentNode + 1
	retVal->elements = malloc(sizeof(void*) * (maxSize + 1));
	if (retVal == NULL) {
		return ERROR_MALLOC();
	}

	return retVal;
}

void destroyHeap(heap* h) {
	free(h->elements);
	free(h);
}

void destroyHeapWithElements(heap* h) {
	for (int i=1; i<=h->size; i++) {
		h->d(h->elements[i]);
	}
	destroyHeap(h);
}

void clearHeap(heap* h) {
	h->size = 0;
	h->nextCellAvailable = 1;
}

bool insertItemInHeap(heap* h, void* item) {
	if (isHeapFull(h)) {
		critical("Cannot add item %p inside heap %p because the heap is full", item, h);
		return false;
	}
	if (containsItemInHeap(h, item)) {
		return false;
	}
	h->elements[h->nextCellAvailable] = item;
	h->nextCellAvailable = getNewNextCellAvailable(h);
	h->size++;
	percolateUp(h, h->nextCellAvailable - 1);
	return true;
}

bool isHeapEmpty(const heap* h) {
	return h->size == 0;
}

bool isHeapFull(const heap* h) {
	return h->size == h->maxSize;
}

void* peekMinItemInHeap(const heap* h) {
	return isHeapEmpty(h) ? NULL : h->elements[1];
}

void* removeMinItemInHeap(heap* h) {
	if (isHeapEmpty(h)) {
		return NULL;
	}

	void* retVal = h->elements[1];
	SWAP(h->elements[1], h->elements[h->nextCellAvailable - 1], void*);
	h->nextCellAvailable--;
	h->size--;
	//percolate down from the root
	percolateDown(h, 1);
	return retVal;
}

int getSizeOfHeap(const heap* h) {
	return h->size;
}

int getMaxSizeOfHeap(const heap* h) {
	return h->maxSize;
}

bool containsItemInHeap(const heap* h, void* item) {
	return containsItemInHeapRecursive(h, 1, item);
}

void* getNthItemInHeap(const heap* h, int i) {
	return h->elements[i];
}

void setBufferStringFunctionInHeap(heap* h, buffer_string bf) {
	h->bf = bf;
}

int getBufferStringOfHeap(const heap* h, char* buffer) {
	int i = 0;
	i += sprintf(&buffer[i], "{ ");
	ITERATE_THROUGH_HEAP(h, payload, void*) {
		i += h->bf(payload, &buffer[i]);
		i += sprintf(&buffer[i], " ");
	}
	i += sprintf(&buffer[i], "}");
	return i;
}

void setClonerFunctionInHeap(heap* h, cloner c) {
	h->c = c;
}

heap* cloneHeap(const heap* h) {
	heap* retVal = initHeap(h->maxSize, h->d, h->relationship);

	retVal->bf = h->bf;
	retVal->c = h->c;

	ITERATE_THROUGH_HEAP(h, payload, void*) {
		insertItemInHeap(retVal, h->c(payload));
	}

	return retVal;
}

void moveHeapElements(heap* dst, heap* src) {
	while (!isHeapEmpty(src)) {
		insertItemInHeap(dst, removeMinItemInHeap(src));
	}
}

static bool containsItemInHeapRecursive(const heap* h, int currentId, void* lookedFor) {
	//we have arrived to a cell in the heap that is not initialized yet. We have reached the end fo the heap for
	//this path
	if (currentId == H_NOID || currentId >= h->nextCellAvailable) {
		return false;
	}

	info("currentId set to %d. NextCellAvailable is %d", currentId, h->nextCellAvailable);
	int i = h->relationship(lookedFor, h->elements[currentId]);
	if (i == 0) {
		//we found our value
		return true;
	}

	if (i < 0) {
		//the element we're look for is smaller than the current node, then we won't bother searching in this subtree, since all the nodes
		//there are bigger than the current node nethertheless.
		return false;
	}

	if (i > 0) {
		//ok, maybe in the subtree there is something
		if (containsItemInHeapRecursive(h, getLeftChild(h, currentId), lookedFor)) {
			return true;
		}
		if (containsItemInHeapRecursive(h, getRightChild(h, currentId), lookedFor)) {
			return true;
		}

		return false;
	}
}

/**
 * @param[in] h the heap analyzed
 * @param[in] index the index to handle
 * @return
 * 	\li true if \c index represents the id of the root;
 * 	\li false otherwise
 */
static bool isIndexRoot(const heap* h, int index) {
	return index == 1;
}

/**
 * @param[in] h the heap analyzed
 * @param[in] index the index to handle
 * @return
 * 	\li true if \c index lead to a leaf node;
 * 	\li false otherwise;
 */
static bool isLeafNode(const heap* h, int index) {
	return (index < h->maxSize) && (index >= h->firstLeafNodeIndex);
}

/**
 * Fetch the id of the parent node of a given node
 *
 * @param[in] h the heap analyzed
 * @param[in] index the position of the cell considered
 * @return
 * 	\li the id of the parent node of the heap;
 * 	\li ::H_NOID if such node doesn't exist
 */
static int getParentCell(const heap* h, int index) {
	return isIndexRoot(h, index) ? H_NOID : index/2;
}

/**
 * Fetch the id of the left child node of a given node
 *
 * @param[in] h the heap analyzed
 * @param[in] index the position of the cell considered
 * @return
 * 	\li the id of the left child node;
 * 	\li ::H_NOID if such node doesn't exist
 */
static int getLeftChild(const heap* h, int index) {
	if (isLeafNode(h, index)) {
		return H_NOID;
	}
	if (h->size < 2*index) {
		return H_NOID;
	}
	return 2*index;
}

/**
 * Fetch the id of the right child node of a given node
 *
 * @param[in] h the heap analyzed
 * @param[in] index the position of the cell considered
 * @return
 * 	\li the id of the right child node;
 * 	\li ::H_NOID if such node doesn't exist
 */
static int getRightChild(const heap* h, int index) {
	if (isLeafNode(h, index)) {
		return H_NOID;
	}
	if (h->size < (2*index + 1)) {
		return H_NOID;
	}
	return (2*index + 1);
}

/**
 * Perform heap percolate operation
 *
 * \note
 * After the operation, the heap is correctly repaired
 *
 * @param[in] h the heap to repair
 * @param[in] index the index when starting to percolate up
 */
static void percolateUp(heap* h, int index) {
	register int parentId = getParentCell(h, index);
	if (parentId == H_NOID) {
		return;
	}
	if (h->relationship(h->elements[index], h->elements[parentId]) < 0 ) {
		//the child is less than the parent. We need to swap
		SWAP(h->elements[index], h->elements[parentId], void*);
	}
	percolateUp(h, parentId);
}

static void percolateDown(heap* h, int parentIndex) {
	if (isLeafNode(h, parentIndex)) {
		return;
	}

	register int lc = getLeftChild(h, parentIndex);
	register int rc = getRightChild(h, parentIndex);
	register int small = 0;

	if (rc == H_NOID) {
		if (lc == H_NOID) {
			//both left and right children don't exist. We do nothing
			return;
		}
		//right child doesn't exist, but left does. We swap parent with the left child
		small = lc;
		finer("parent %d(%p) lc[small] = %d(%p) size = %d", parentIndex, h->elements[parentIndex], lc, h->elements[lc], h->size);
	} else {
		//both children exist. We need to determin which one we need to swap with.
		info("parent %d(%p) lc = %d(%p) rc = %d(%p) size = %d", parentIndex, h->elements[parentIndex], lc, h->elements[lc], rc, h->elements[rc], h->size);
		if (h->relationship(h->elements[lc], h->elements[rc]) < 0) {
			//left is less than right
			small = lc;
		} else {
			//right is less than left
			small = rc;
		}
	}
	if (h->relationship(h->elements[parentIndex], h->elements[small]) < 0) {
		return;
	}
	SWAP(h->elements[small], h->elements[parentIndex], void*);
	percolateDown(h, small);
}

static int getNewNextCellAvailable(const heap* h) {
	return h->nextCellAvailable + 1;
}


