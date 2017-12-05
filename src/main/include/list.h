/*
 * \file list.h
 *
 * Represents a list. The list implemented is a simple one (not double-linked).
 * In the memory, it might resemble to:
 *
 * \dot
 * 	digraph {
 * 		A;
 * 		B;
 * 		C;
 *
 * 		A -> B -> C;
 * 	}
 * \enddot
 *
 *
 *
 *  Created on: May 17, 2017
 *      Author: koldar
 */

#ifndef LIST_H_
#define LIST_H_

#include <stdlib.h>

#include "stdbool.h"
#include "macros.h"
#include "errors.h"


typedef struct list list;
typedef struct list_cell list_cell;

/**
 * a list of integers.
 * Integers are stored inside the pointer to a structure itself
 */
typedef list int_list;

list_cell* getNextInListCell(const list_cell* cell);

void* getPayloadInListCell(const list_cell* cell);

list_cell* getHeadInList(const list* l);

/**
 * Initialize a new list inside the memory.
 *
 * Use ::destroyList or ::destroyListWithElement to release the memory from the list
 *
 * @return the list requested
 */
list* initList();

void destroyList(list* l);

void destroyListWithElement(list* l, void(*destructor)(void*));

void clearList(list* l);

void addHeadInList(list* l, void* el);

void addTailInList(list* l, void* el);

int getLengthOfList(const list* l);

/**
 * Adds all the elements of \c src into \c dst
 *
 * \note
 * After this function, \c src will be empty
 *
 * @param[inout] dst the list that will accept all the elements inside \c scc;
 * @param[inout] src the list whose elements will be tranferred to \c
 */
void moveListContents(list* restrict dst, list* restrict src);

/**
 * Get the tail of the list
 *
 * @param [in] l the list to handle
 * @return the tail of the list or NULL if the list is empty
 */
void* getLastElementOfList(const list* l);

/**
 *
 * Check if the given list is empty
 *
 * @param[in] l the list to analyze
 * @return
 * 	\li true if \c l is empty;
 * 	\li false otherwises
 */
bool isEmptyList(const list* l);

/**
 * Get and remove the head from the list \c l
 *
 * @param[in] l the list involved
 * @return
 * 	\li the payload at the head of the list;
 * 	\li NULL if the list is empty;
 */
void* popFromList(list* l);

/**
 * Fetch the head of the list
 *
 * @param[in] l the list to handle
 * @return
 * 	\li the head fo the list;
 * 	\li NULL if \c l is empty;
 */
void* getHeadOfList(const list* l);

/**
 * Fetch the tail of the list
 *
 * @param[in] l the list to analyze
 * @return
 * 	\li the tail of the list;
 * 	\li NULL if \c l is empty
 */
void* getTailOfList(const list* l);

/**
 * Get the n-th element of the list
 *
 * \attention
 * This method is **highly** inefficient and it should be used only for debugging purposes, not in production!
 *
 * The operation take \f$O(n)\f$ time
 *
 * @param[in] list the list to analyze
 * @param[in] index the value in the \c index th position
 * @return
 * 	\li the payload
 * 	\li NULL if the length of \c list is smaller than \c index itself;
 */
void* getNthElementOfList(const list* list, int index);

/**
 * Removes the cell \c cellToRemove in the list
 *
 * \attention
 * You should use this function only inside ::VARIABLE_ITERATE_ON_LIST. Other uses may (and often will) lead to **undefined behaviour**!
 *
 * This function will allow you to dynamically remove elements inside a list when you're iterating it.
 * A typical example might be:
 *
 * \code
 * VARIABLE_ITERATE_ON_LIST(&list, previous, cell, payload) {
 * 	if (conditionToPayload()) {
 * 		removeElementInList(&list, &previous, cell);
 * 	}
 * }
 * \endcode
 *
 * @param[in] l the list to change
 * @param[inout] previousCell the cell that is positioned just before the one to remove. After this, previousCell pointer will contain NULL
 * @param[in] cellToRemove the cell you want to remove
 *
 */
void removeElementInListCell(list* l,list_cell** restrict previousCell, list_cell* restrict cellToRemove);

/**
 * Allows you to transparently iterate through a list
 *
 * This macro is *slightly* faster than ::VARIABLE_ITERATE_ON_LIST but it doesn't allow you to
 * safely remove ::list_cell while iterating
 *
 * @param[in] _l a double point to the list you want to iterate through;
 * @param[in] cell name of the variable that will represent the cell under analysis
 * @param[in] _payload name of the variable representing <tt>cell->payload</tt>
 * @param[in] type type of the variable _payload
 */
#define ITERATE_ON_LIST(_l, cell, _payload, type) 												\
		list* UV(l) = (_l);																		\
		list_cell* cell = getHeadInList(UV(l)); 												\
		list_cell* UV(next) = NULL;																\
		type _payload = NULL;																	\
		if (cell != NULL) {																		\
			_payload = getPayloadInListCell(cell);												\
			if (getNextInListCell(cell) != NULL) {												\
				UV(next) = getNextInListCell(cell);												\
			}																					\
		}																						\
		for (																					\
				;																				\
				cell != NULL;																	\
				cell = UV(next), 																\
				UV(next) = (cell != NULL) ? getNextInListCell(cell) : NULL, 					\
				_payload = (cell != NULL) ? getPayloadInListCell(cell) : NULL					\
		)

/**
 * Allows you to transparently iterate through a list
 *
 * This macro is *slightly* slower than ::ITERATE_ON_LIST but allows you to
 * safely remove ::list_cell while iterating.
 *
 * A typical example might be:
 *
 * \code
 * VARIABLE_ITERATE_ON_LIST(&list, previous, cell, payload) {
 * 	if (conditionToPayload()) {
 * 		removeElementInList(&list, &previous, cell);
 * 	}
 * }
 * \endcode
 *
 * @param[in] _l a double point to the list you want to iterate through;
 * @param[in] previousCell name of the variable that will represent the cell just analyzed.
 * @param[in] cell name of the variable that will represent the cell under analysis
 * @param[in] _payload name of the variable representing <tt>cell->payload</tt>
 * @param[in] type type of the variable _payload
 */
#define VARIABLE_ITERATE_ON_LIST(_l, previousCell, cell, _payload, type) \
		list* UV(l) = (_l);\
		list_cell* UV(next) = NULL;\
		type _payload = NULL;\
		if (getHeadInList(UV(l)) != NULL) {\
			_payload = getPayloadInListCell(getHeadInList(UV(l)));\
			if (getNextInListCell(getHeadInList(UV(l))) != NULL) {\
				UV(next) = getNextInListCell(getHeadInList(UV(l)));\
			}\
		}\
		for (\
				list_cell \
				*previousCell = NULL, \
				*UV(previousCellTmp) = NULL, \
				*cell = getHeadInList(UV(l)) \
				; \
				cell != NULL \
				; \
				UV(previousCellTmp) = previousCell == NULL ? UV(previousCellTmp) : cell, \
				previousCell = UV(previousCellTmp), \
				cell = UV(next), \
				UV(next) = (cell != NULL) ? getNextInListCell(cell) : NULL, \
				_payload = (cell != NULL) ? getPayloadInListCell(cell) : NULL\
		)


#endif /* LIST_H_ */
