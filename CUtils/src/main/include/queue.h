/*
 * queue.h
 *
 *  Created on: Aug 2, 2017
 *      Author: koldar
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdbool.h>
#include "typedefs.h"

/**
 * A type for any function giving something a value
 */
typedef int (*evaluator_function)(void* x);

typedef struct queue queue;

queue* initQueue(evaluator_function f, bool cacheEvaluations, destructor d);

void destroyQueue(queue* q);

void addItemInQueue(queue* q, void* data);

void* _peekItemFromQueue(const queue* q);

void* _popItemFromQueue(queue* q);

int getSizeOfQueue(const queue* q);

bool isQueueEmpty(const queue* q);

void clearQueue(queue* q);

void destroyQueueWithElements(queue* q);

bool containsItemInQueue(const queue* q, void* data);

//_QUEUE_H_AVOID_MACROS is defined in queue.c: in this way the source code can use and define the underlying method on the macros
#ifndef _QUEUE_H_AVOID_MACROS
#	define peekItemFromQueue(q, type) ((type)_peekItemFromQueue(q))
#	define popItemFromQueue(q, type) ((type)_popItemFromQueue(q))
#endif

#endif /* QUEUE_H_ */
