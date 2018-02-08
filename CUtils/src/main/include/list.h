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

#include "stdbool.h"
#include <stdlib.h>
#include "typedefs.h"
#include "macros.h"

typedef struct list_cell {
	///represents the paylaod inside this cell of the list
	void* payload;
	///a pointer to the next element of the list. Can be NULL
	struct list_cell* next;
} list_cell;

typedef struct {
	///number of elements inside the list. Allows count operation to be O(1)
	int size;
	///pointer to the first element of the list. Can be NULL
	list_cell* head;
	///pointer to the last element of the list. Can be NULL
	list_cell* tail;
} list;

/**
 * a list of integers.
 * Integers are stored inside the pointer to a structure itself
 */
typedef list int_list;
/**
 * A list of strings
 * The payload of the each list cell is a pointer to the string
 */
typedef list string_list;

/**
 * Initialize a new list inside the memory.
 *
 * Use ::destroyList or ::destroyListWithElement to release the memory from the list
 *
 * @return the list requested
 */
list* initList();

void destroyList(list* l);

void destroyListWithElement(list* l, destructor d);

/**
 * Clear the whole list
 *
 * \post
 * 	\li list is empty
 *
 * \note
 * the payload won't be freed at all. See ::clearListWithElements instead
 *
 * If this list is the only one referencing those elements, doing this will create memory leaks!
 *
 * @param[inout] l the list to free
 */
void clearList(list* l);

/**
 * Clear the whole list with elements
 *
 * \post
 * 	\li list is empty;
 * 	\li all the payload is deleted as well
 *
 *
 * If this list is the only one referencing those elements, doing this **won't** create memory leaks!
 *
 * @param[inout] l the list to free
 */
void clearListWithElements(list* l, destructor d);

/**
 * Clone the given list
 *
 * The elements inside the list are copy **by value**: each element inside the list will be cloned
 * with \c c.
 * After this operation, if you release from memory the data inside the original list, the clone will
 * **not** become faulty
 *
 * @param[in] l the list to clone
 * @param[in] c the function used to clone each element of the list
 * @return the clone
 */
list* cloneListWithElements(const list* l, cloner c);

/**
 * Clone the given list
 *
 * The elements inside the list are copy **by reference**.
 * After this operation, if you release from memory the data inside the original list, the clone will
 * become faulty
 *
 * @param[in] l the list to clone
 * @return the clone
 */
list* cloneList(const list* l);

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
 * Alias for ::getHeadOfList
 *
 * @param[in] l the list to handle
 * @return
 * 	\li the head fo the list;
 * 	\li NULL if \c l is empty;
 */
void* peekFromList(const list* l);

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
 * Find an element inside the list
 *
 * The function will retrieve the **first** element satisfying the function \c f
 *
 * \note
 * This operation is O(n)
 *
 * @param[in] l the list under analysis
 * @param[in] f a function that returns true if an element satisfy your condition
 * @return
 * 	\li the first element satifying your condition;
 * 	\li NULL otherwise;
 */
void* findElementInList(const list* l, finder f);

/**
 * Remove the element from the list.
 *
 * Nothing is done is the element is not found within the list.
 * Keep in mind that the element is **not** freed from the memory
 *
 * @param[inout] l the list to alter
 * @param[in] f a function used to check if the element has been found
 * @return
 * 	\li true if the element has been found inside the list;
 * 	\li false otherwise
 */
bool removeElementInList(list* l, finder f);

/**
 * Remove the element from the list.
 *
 * Nothing is done is the element is not found within the list.
 * **Keep in mind that the element is freed from the memory**
 *
 * @param[inout] l the list to alter
 * @param[in] f a function used to check if the element has been found
 * @param[in] d a function used to release the memory from the element.
 * @return
 * 	\li true if the element has been found inside the list;
 * 	\li false otherwise
 */
bool removeAndDestroyElementInList(list* l, finder f, destructor d);

/**
 * like ::findElementInList but it look for an item whose reference is the one specified by \c obj
 *
 * \note
 * Of course this is not powerful like ::findElementInList, but it can work in many cases
 * (for example when the payload is inside the cell pointer itself)
 *
 * @param[in] l the list where we need to look at
 * @param[in] obj the reference we're looking for
 * @return
 * 	\li true if the reference \c obj is inside the list;
 * 	\li false otherwise
 */
bool containsItemInListByReference(list* l, void* obj);

/**
 * like ::removeElementInList, but it removes the object if the reference is the same.
 *
 * \note
 * Of course this is not powerful like ::removeElementInList, but it can work in many cases
 * (for example when the payload is inside the cell pointer itself)
 *
 * If the obj is not found, the function does nothing
 * If multiple instances of \c obj are presents inside the list, only the first is removed.
 *
 * \note
 * No release of the memory occupied by the payload is done
 *
 * @param[inout] l the list to handle
 * @param[in] obj the object to find
 * @return
 * 	\li true if the object has been removed;
 * 	\li false otherwise;
 */
bool removeItemInListByReference(list* l, void* obj);

/**
 * Generate a string that represents the list
 *
 * Be sure to **free** the return value fo this function from the memory after use!
 *
 * @param[in] list the list to print
 * @param[in] f function that accepts a value within the list and return a string representation
 * @return a string representation of the list
 */
const char* getStringOfList(const list* list, stringify f);

/**
 * Get a string representation of the given list
 *
 * @param[in] lst the list to represents
 * @param[in] f a function that will compute the string representation of the payload of the list
 */
int getBufferStringOfList(const list* lst, char* buffer, buffer_string f);

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
 * Removes the  n-th element of the list
 *
 * \attention
 * This method is **highly** inefficient and it should be used only for debugging purposes, not in production!
 *
 * The operation takes \f$O(n)\f$ time
 *
 * @param[inout] lst the list to handle
 * @param[in] index the index of the cell to remove
 * @return
 * 	\li true if the element is removed;
 * 	\li false otherwise;
 */
bool removeNthElementOfList(list* lst, int index);

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
 * **Alter** the list order by scrambling the elements
 *
 * For example if the list is \f$[1,2,3]\f$ after calling this function the list may have become \f$[3,1,2]\f$
 *
 * @param[inout] l the list to scramble
 */
void scrambleList(list* l);

/**
 * **Alter** the list order by order the element in a certain way
 *
 * @param[inout] l the list to change
 * @param[in] order the function to use to establish which will be the order to use to reorganize the list itself
 */
void sortList(list* l, orderer order);

/**
 * Get a random item in the given list
 *
 * @param[in] l the list where we need to pick from
 * @return
 * 	\li the item retrieved;
 * 	\li NULL if the list is empty;
 */
void* pickRandomItemInList(const list* l);

/**
 * Store a list inside a file pointer
 *
 * \pre
 * 	\li \c f open in binary mode;
 *
 * @param[inout] f the file that will accept a serialization of the list
 * @param[in] l the list to store
 * @param[in] serializer the function to use to serialize the payload of the list
 */
void storeListInFile(FILE* f, const list* l, object_serializer serializer);

/**
 * Load a list from a file
 *
 * \pre
 * 	\li \c f open for reading binary
 *
 * @param[inout] f the file to read from
 * @param[in] deserializer the function to use to deserialize the paylaod of the list
 * @return
 * 	\li the instance of list read;
 * 	\li NULL if we couldn't read the file at all;
 */
list* loadListFromFile(FILE* f, object_deserializer deserializer);

/**
 * Check if the given cell is the last element of the list
 *
 * The function is useful with ::ITERATE_ON_LIST, for example
 *
 * @code
 * ITERATE_ON_LIST(l, cell, payload, void) {
 * 	if (isLastItemInList(l, cell)) {
 * 		printf("Last element!\n");
 * 	} else {
 * 		printf("Not last element!\n");
 * 	}
 * }
 * @endcode
 *
 * @param[in] l the list involved
 * @param[in] cell the cell to consider
 * @return
 * 	\li true if this is the last cell in the list;
 * 	\li false otherwise
 */
bool isLastItemInList(const list* l, const list_cell* cell);

/**
 * Merges 2 lists
 *
 * Suppose:
 * <p>list1: 5 -> 6 -> 7</p>
 * <p>list2: 1 -> 2 -> 3</p>
 *
 * Applying this function like <tt>mergeLists(list1, list2)</tt> will **modify** \c list1 to be:
 * <p>list1: 5 -> 6 -> 7 -> 1 -> 2 -> 3</p>.
 *
 * After this
 *
 * @param[inout] dst the list where all the items will be put
 * @param[in] src the list that needs to be merged with \c dst
 *
 */
//TODO continue from here void fastMergeLists(list* restrict dst, const list* restrict src);

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
#define ITERATE_ON_LIST(_l, cell, _payload, type) \
		list* UV(l) = (_l);\
		list_cell* cell = UV(l)->head;\
		list_cell* UV(next) = NULL;\
		type* _payload = NULL;\
		if (UV(l)->head != NULL) {\
			_payload = UV(l)->head->payload;\
			if (UV(l)->head->next != NULL) {\
				UV(next) = UV(l)->head->next;\
			}\
		}\
		for (\
				;\
				cell != NULL;\
				cell = UV(next), \
				UV(next) = (cell != NULL) ? cell->next : NULL, \
				_payload = (cell != NULL) ? cell->payload : NULL\
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
		type* _payload = NULL;\
		int UV(prevLength) = getLengthOfList(UV(l));\
		if (UV(l)->head != NULL) {\
			_payload = UV(l)->head->payload;\
			if (UV(l)->head->next != NULL) {\
				UV(next) = UV(l)->head->next;\
			}\
		}\
		for (\
				list_cell \
				*previousCell = NULL, \
				*UV(previousCellTmp) = NULL, \
				*cell = UV(l)->head \
				; \
				cell != NULL \
				; \
				UV(previousCellTmp) = (UV(prevLength) != getLengthOfList(UV(l))) ? UV(previousCellTmp) : cell, \
				previousCell = UV(previousCellTmp), \
				UV(prevLength) = getLengthOfList(UV(l)), \
				cell = UV(next), \
				UV(next) = (cell != NULL) ? cell->next : NULL, \
				_payload = (cell != NULL) ? cell->payload : NULL\
		)


#endif /* LIST_H_ */
