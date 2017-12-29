/*
 * \file list.h
 *
 * Represents a list. The implemented list is a simple one (not double-linked).
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
#include <stdbool.h>

#include "typedefs.h"
#include "macros.h"
#include "errors.h"


typedef struct list ct_list_t;
typedef struct list_cell ct_list_cell_t;

ct_list_cell_t* ct_next_list_cell(const ct_list_cell_t* cell);

void* ct_list_cell_payload(const ct_list_cell_t* cell);

ct_list_cell_t* ct_list_head_cell(const ct_list_t* l);

/**
 * Initialize a new list inside the memory.
 *
 * Use ::ct_destroy_list or ::ct_destroy_list_with_elements to release the memory from the ct_list_t
 *
 * @return the requested list
 */
ct_list_t* ct_init_list();

void ct_destroy_list(ct_list_t* l);

void ct_destroy_list_with_elements(ct_list_t* l, ct_destructor_t d);

void ct_clear_list(ct_list_t* l);

void ct_add_head_in_list(ct_list_t* l, void* el);

void ct_add_tail_in_list(ct_list_t* l, void* el);

int ct_list_length(const ct_list_t* l);

/**
 * Adds all the elements of \c src into \c dst
 *
 * \note
 * After this function, \c src will be empty
 *
 * @param[inout] dst the list that will accept all the elements inside \c scc;
 * @param[inout] src the list whose elements will be tranferred to \c
 */
void ct_move_list_contents(ct_list_t* restrict dst, ct_list_t* restrict src);

/**
 * Get the tail of the list
 *
 * @param [in] l the list to handle
 * @return the tail of the list or NULL if the list is empty
 */
void* ct_list_last_element(const ct_list_t* l);

/**
 *
 * Check if the given list is empty
 *
 * @param[in] l the list to analyze
 * @return
 * 	\li true if \c l is empty;
 * 	\li false otherwises
 */
bool ct_is_list_empty(const ct_list_t* l);

/**
 * Get and remove the head from the list \c l
 *
 * @param[in] l the involved list
 * @return
 * 	\li The payload at the head of the list;
 * 	\li NULL Wheter the list is empty;
 */
void* ct_pop_from_list(ct_list_t* l);

/**
 * Fetch the head of the list
 *
 * @param[in] l the list to handle
 * @return
 * 	\li the head fo the list;
 * 	\li NULL if \c l is empty;
 */
void* ct_list_head(const ct_list_t* l);

/**
 * Fetch the tail of the list
 *
 * @param[in] l the list to analyze
 * @return
 * 	\li the tail of the list;
 * 	\li NULL if \c l is empty
 */
void* ct_list_tail(const ct_list_t* l);

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
void* ct_get_list_element(const ct_list_t* list, int index);

/**
 * Removes the cell \c cell_to_remove in the list
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
 * 		ct_remove_element_list_cell(&list, &previous, cell);
 * 	}
 * }
 * \endcode
 *
 * @param[in] l the list to change
 * @param[inout] previous_cell the cell that is positioned just before the one to remove. After this, previous_cell pointer will contain NULL
 * @param[in] cell_to_remove the cell you want to remove
 *
 */
void ct_remove_element_list_cell(ct_list_t* l, ct_list_cell_t** restrict previous_cell, ct_list_cell_t* restrict cell_to_remove);

/**
 * Allows you to transparently iterate through a list
 *
 * This macro is *slightly* faster than ::VARIABLE_ITERATE_ON_LIST but it doesn't allow you to
 * safely remove ::ct_list_cell_t while iterating
 *
 * @param[in] _l a double point to the list you want to iterate through;
 * @param[in] cell name of the variable that will represent the cell under analysis
 * @param[in] _payload name of the variable representing <tt>cell->payload</tt>
 * @param[in] type type of the variable _payload
 */
#define ITERATE_ON_LIST(_l, cell, _payload, type) 												\
		const ct_list_t* UV(l) = (_l);															\
		ct_list_cell_t* cell = ct_list_head_cell(UV(l)); 										\
		ct_list_cell_t* UV(next) = NULL;														\
		type _payload = NULL;																	\
		if (cell != NULL) {																		\
			_payload = ct_list_cell_payload(cell);												\
			if (ct_next_list_cell(cell) != NULL) {												\
				UV(next) = ct_next_list_cell(cell);												\
			}																					\
		}																						\
		for (																					\
				;																				\
				cell != NULL;																	\
				cell = UV(next), 																\
				UV(next) = (cell != NULL) ? ct_next_list_cell(cell) : NULL, 					\
				_payload = (cell != NULL) ? ct_list_cell_payload(cell) : NULL					\
		)

/**
 * Allows you to transparently iterate through a list
 *
 * This macro is *slightly* slower than ::ITERATE_ON_LIST but allows you to
 * safely remove ::ct_list_cell_t while iterating.
 *
 * A typical example might be:
 *
 * \code
 * VARIABLE_ITERATE_ON_LIST(&list, previous, cell, payload) {
 * 	if (conditionToPayload()) {
 * 		ct_remove_element_list_cell(&list, &previous, cell);
 * 	}
 * }
 * \endcode
 *
 * @param[in] _l a double point to the ct_list_t you want to iterate through;
 * @param[in] previous_cell name of the variable that will represent the cell just analyzed.
 * @param[in] cell name of the variable that will represent the cell under analysis
 * @param[in] _payload name of the variable representing <tt>cell->payload</tt>
 * @param[in] type type of the variable _payload
 */
#define VARIABLE_ITERATE_ON_LIST(_l, previous_cell, cell, _payload, type) 						\
		ct_list_t* UV(l) = (_l);																\
		ct_list_cell_t* UV(next) = NULL;														\
		type _payload = NULL;																	\
		if (ct_list_head_cell(UV(l)) != NULL) {													\
			_payload = ct_list_cell_payload(ct_list_head_cell(UV(l)));							\
			if (ct_next_list_cell(ct_list_head_cell(UV(l))) != NULL) {							\
				UV(next) = ct_next_list_cell(ct_list_head_cell(UV(l)));							\
			}																					\
		}																						\
		for (																					\
				ct_list_cell_t 																	\
				*previous_cell = NULL, 															\
				*UV(previous_cell_tmp) = NULL, 													\
				*cell = ct_list_head_cell(UV(l)) 												\
				; 																				\
				cell != NULL 																	\
				; 																				\
				UV(previous_cell_tmp) = previous_cell == NULL ? UV(previous_cell_tmp) : cell, 	\
				previous_cell = UV(previous_cell_tmp), 											\
				cell = UV(next), 																\
				UV(next) = (cell != NULL) ? ct_next_list_cell(cell) : NULL, 					\
				_payload = (cell != NULL) ? ct_list_cell_payload(cell) : NULL					\
		)


#endif /* LIST_H_ */
