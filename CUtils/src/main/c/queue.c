/*
 * queue.c
 *
 *  Created on: Aug 2, 2017
 *      Author: koldar
 */

#define _QUEUE_H_AVOID_MACROS

#include "queue.h"
#include "macros.h"
#include "log.h"
#include <stdlib.h>
#include <math.h>

// a queue is implemented as an infinite heap

typedef struct queue_cell {
	void* payload;
	int cache;
	struct queue_cell* parent;
	struct queue_cell* left;
	struct queue_cell* right;
} queue_cell;


struct queue {
	queue_cell* min;
	int size;
	destructor d;
	evaluator_function eval;
	bool cacheEvaluations;
	int nextCellAvailable;
};

static queue_cell* initQueueCell();
static queue_cell* destroyQueueCell(queue_cell* qc);
static queue_cell* destroyQueueCellWithElement(queue_cell* qc, destructor d);
static int evaluateQueueCell(queue* q, queue_cell* qc);
static bool isParent(queue* q, queue_cell* qc);
static bool isLeaf(queue_cell* qc);
static void percolateUp(queue* q, queue_cell* qc);
static void percolateDown(queue* q, queue_cell* qc);
static void gotoCellWithId(const queue* q, int idToRead, queue_cell** pointerOfCellReached, queue_cell** pointerOfParentCell, queue_cell*** pointerOfParentReachingNode);
static void _clearQueue(queue* q, queue_cell* qc, bool destroyPayload);
static bool containsItemInQueueRecursive(const queue* q, const queue_cell* qc, int evalData, void* data);

queue* initQueue(evaluator_function f, bool cacheEvaluations, destructor d) {
	queue* retVal = malloc(sizeof(queue));
	if (retVal == NULL) {
		ERROR_MALLOC();
	}

	retVal->d = d;
	retVal->eval = f;
	retVal->min = NULL;
	retVal->size = 0;
	retVal->cacheEvaluations = cacheEvaluations;
	retVal->nextCellAvailable = 1;

	return retVal;
}

void destroyQueue(queue* q) {
	clearQueue(q);
	free(q);
}

void addItemInQueue(queue* q, void* data) {
	//I need to position the new payload on the last leaf

	//level is the power of 2 of every node in the level
	//i.e level=3 --> ids of nodes will be 8,9,10,11,12,13,14,15
	//right-most node will be 2^n-1
	//left-most node will be 2^n
	//at level i, all the nodes in the left branch has the i-th bit (counting from the right) set to 0
	//at level i, all the nodes in the right branch has the i-th bit (counting from the right) set to 1
	int level = 0;
	int qcId = 1;
	queue_cell* qc = NULL;
	queue_cell* parent = NULL;
	queue_cell** parentPointer = NULL;

	//we do that now because gotoCellWithId needs that
	q->size += 1;

	gotoCellWithId(q, q->nextCellAvailable, &qc, &parent, &parentPointer);

#ifdef DEBUG
			if (qc != NULL) {
				ERROR_IS_ALREADY_PRESENT(qc, "heap", "%p");
			}
#endif

	qc = initQueueCell();
	*parentPointer = qc;

	qc->payload = data;
	if (isQueueEmpty(q)) {
		qc->parent = NULL;
	} else {
		qc->parent = parent;
	}
	if (q->cacheEvaluations) {
		qc->cache = q->eval(qc->payload);
	}

	q->nextCellAvailable += 1;

	percolateUp(q, qc);
}

void* _peekItemFromQueue(const queue* q) {
	return q->min != NULL ? q->min->payload : NULL;
}

void* _popItemFromQueue(queue* q) {
	void* retVal = NULL;
	queue_cell* min = NULL;

	//queue is empty. Return nothing
	if (isQueueEmpty(q)) {
		ERROR_NOT_SUCH_OBJECT("queue empty", q);
	}

	q->size -= 1;
	q->nextCellAvailable -= 1;
	min = q->min;
	retVal = q->min->payload;

	//queue has one element, return such element
	//remember: we have just remove size by 1!
	if (q->size == 0) {
		destroyQueueCell(q->min);
		q->min = NULL;
		return retVal;
	}
	//we have at least 2 values inside the queue

	queue_cell* qc;
	queue_cell* parent;
	queue_cell** pointerOfParent;

	gotoCellWithId(q, q->nextCellAvailable, &qc, &parent, &pointerOfParent);

	SWAP(q->min->payload, qc->payload, void*);
	if (q->cacheEvaluations) {
		SWAP(q->min->cache, qc->cache, int);
	}

	//min still points to the top of the heap and qc to the bottom. We need to remove the bottom
	//destroy the cell of the min (not used anymore)
	if (qc->parent->left == qc) {
		qc->parent->left = NULL;
	} else {
		qc->parent->right = NULL;
	}
	destroyQueueCell(qc);

	percolateDown(q, q->min);

	return retVal;
}

int getSizeOfQueue(const queue* q) {
	return q->size;
}

bool isQueueEmpty(const queue* q) {
	return q->size == 0;
}

void clearQueue(queue* q) {
	_clearQueue(q, q->min, false);
	q->size = NULL;
	q->nextCellAvailable = 1;
}

void destroyQueueWithElements(queue* q) {
	_clearQueue(q, q->min, true);
	free(q);
}

bool containsItemInQueue(const queue* q, void* data) {
	return containsItemInQueueRecursive(q, q->min, q->eval(data), data);
}

static bool containsItemInQueueRecursive(const queue* q, const queue_cell* qc, int evalData, void* data) {
	if (qc == NULL) {
		return false;
	}
	if (qc->payload == data) {
		return true;
	}

	if (qc->left != NULL) {
		int evalLeft = evaluateQueueCell(q, qc->left);
		if (evalData > evalLeft) {
			if (containsItemInQueueRecursive(q, qc->left, evalData, data)) {
				return true;
			}
		}
	}

	if (qc->right != NULL) {
		int evalRight = evaluateQueueCell(q, qc->right);
		if (evalData > evalRight) {
			if (containsItemInQueueRecursive(q, qc->right, evalData, data)) {
				return true;
			}
		}
	}

	return false;
}

static void gotoCellWithId(const queue* q, int idToRead, queue_cell** pointerOfCellReached, queue_cell** pointerOfParentCell, queue_cell*** pointerOfParentReachingNode) {
	int qcId = 1;
	queue_cell** parentPointer = &q->min;
	queue_cell* qc = q->min;
	//level of the node under analysis
	int level = 0;
	int size = (int) floor(log2(idToRead));

	*pointerOfParentCell = NULL;
	*pointerOfCellReached = NULL;
	*pointerOfParentReachingNode = NULL;

	//level is the power of 2 of every node in the level
	//i.e level=3 --> ids of nodes will be 8,9,10,11,12,13,14,15
	//right-most node will be 2^n-1
	//left-most node will be 2^n
	//at level i, all the nodes in the left branch has the i-th bit (counting from the right) set to 0
	//at level i, all the nodes in the right branch has the i-th bit (counting from the right) set to 1

	while(true) {
		if (qcId == idToRead) {
			//we need to put the new element here
			*pointerOfCellReached = qc;
			*pointerOfParentReachingNode = parentPointer;
			return;
		} else if ((idToRead & (1 << (size - level - 1))) > 0) {
			parentPointer = &qc->right;
			*pointerOfParentCell = qc;
			qc = qc->right;
			qcId = 2 * qcId + 1;
		} else {
			parentPointer = &qc->left;
			*pointerOfParentCell = qc;
			qc = qc->left;
			qcId = 2 * qcId + 0;
		}
		level++;
	}
}

static void _clearQueue(queue* q, queue_cell* qc, bool destroyPayload) {
	if (qc == NULL) {
		return;
	}

	_clearQueue(q, qc->left, destroyPayload);
	_clearQueue(q, qc->right, destroyPayload);
	if (destroyPayload) {
		q->d(qc->payload);
	}
	destroyQueueCell(qc);
}


/**
 * @param[in] qc the ::queue_cell to analyze
 * @return
 * 	\li true if \c qc is the top of the heap;
 * 	\li false otherwise;
 */
static bool isParent(queue* q, queue_cell* qc) {
	return q->min == qc;
}

static bool isLeaf(queue_cell* qc) {
	return qc->left == NULL;
}

static int evaluateQueueCell(queue* q, queue_cell* qc) {
	if (!q->cacheEvaluations) {
		critical("computing eval");
		return q->eval(qc->payload);
	}
	return qc->cache;
}

static void percolateUp(queue* q, queue_cell* qc) {
	if (isParent(q, qc)) {
		return;
	}

	int evalThis = evaluateQueueCell(q, qc);
	int evalParent = evaluateQueueCell(q, qc->parent);

	if (evalThis < evalParent) {
		SWAP(qc->payload, qc->parent->payload, void*);
		if (q->cacheEvaluations) {
			SWAP(qc->cache, qc->parent->cache, int);
		}
	}

	percolateUp(q, qc->parent);
}

static void percolateDown(queue* q, queue_cell* qc) {
	if (qc == NULL) {
		return;
	}
	if (isLeaf(qc)) {
		return;
	}
	queue_cell* small;

	if (qc->left == NULL && qc->right == NULL) {
		return;
	}

	int evalLeft;
	int evalRight;
	int evalSmall;

	if (qc->right == NULL) {
		small = qc->left;
		evalSmall = evaluateQueueCell(q, qc->left);
	} else {
		evalLeft = evaluateQueueCell(q, qc->left);
		evalRight = evaluateQueueCell(q, qc->right);

		if (evalLeft < evalRight) {
			evalSmall = evalLeft;
			small = qc->left;
		} else {
			evalSmall = evalRight;
			small = qc->right;
		}
	}

	int evalCurrent = evaluateQueueCell(q, qc);
	if (evalCurrent < evalSmall) {
		return;
	}

	SWAP(small->payload, qc->payload, void*);
	if (q->cacheEvaluations) {
		SWAP(small->cache, qc->cache, int);
	}

	percolateDown(q, small);
}

static queue_cell* initQueueCell() {
	queue_cell* retVal = malloc(sizeof(queue_cell));
	if (retVal == NULL) {
		ERROR_MALLOC();
	}

	retVal->left = NULL;
	retVal->cache = 0;
	retVal->right = NULL;
	retVal->payload = NULL;
	retVal->parent = NULL;

	return retVal;
}

static queue_cell* destroyQueueCell(queue_cell* qc) {
	free(qc);
}

static queue_cell* destroyQueueCellWithElement(queue_cell* qc, destructor d) {
	d(qc->payload);
	free(qc);
}
