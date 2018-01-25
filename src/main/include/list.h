/**
 * @file
 *
 * Basic linked-list implementation
 *
 * The list implemented is a simple one (**not** double-linked).
 * In the memory, it might resemble to:
 *
 * @dot
 * digraph {
 * 	rankdir="LR";
 * 	A -> B -> C;
 * }
 * @enddot
 *
 * The list is composed by "entries": each entry contains a pointer to the value in the list and a pointer to the next entry
 *
 * @author koldar
 * @date May 17, 2017
 */

#ifndef LIST_H_
#define LIST_H_

#include <stdlib.h>
#include <stdbool.h>

#include "typedefs.h"

struct ct_list;
typedef struct ct_list ct_list_o;

struct ct_list_entry;
typedef struct ct_list_entry ct_list_entry_o;

/**
 * Initialize a new list inside the memory.
 *
 * Use ::ct_list_destroy or ::ct_list_destroy_with_elements to release the memory from the ct_list_o
 *
 * @return the requested list
 */
ct_list_o* ct_list_init();

/**
 * Destroy the list
 *
 * \note
 * The payload within it won't be release from the memory **at all**!
 *
 * @param[inout] l the list to handle
 * @see ct_list_destroy_with_elements
 */
void ct_list_destroy(const ct_list_o* l);

/**
 * like ::ct_list_destroy but it releases from the memory all the elements within the list as well
 *
 * @param[inout] l the list to handle
 * @param[in] d a function used to dispose all values within the list from memory
 */
void ct_list_destroy_with_elements(const ct_list_o* l, ct_destroyer_c d);

/**
 * Remove all the payloads from the list
 *
 * \note
 * List items won't be removed from memory at all
 *
 * @param[inout] l the list to handle
 */
void ct_list_clear(ct_list_o* l);

/**
 * Adds a new element at the beginning of the list
 *
 * @param[inout] l the list we need to prepend a new item in
 * @param[in] el the item to prepend
 */
void ct_list_add_head(ct_list_o* l, const void* el);

/**
 * Adds a new element at the end of the list
 *
 * @param[inout] l the list we need to append a new item in
 * @param[in] el the item to append
 */
void ct_list_add_tail(ct_list_o* l, const void* el);

/**
 * The size of the list
 *
 * @param[in] l the list to handle
 * @return the length of the list
 */
int ct_list_size(const ct_list_o* l);

/**
 * Adds all the elements of \c src into \c dst
 *
 * \note
 * After this function, \c src will be empty
 *
 * @param[inout] dst the list that will accept all the elements inside \c scc;
 * @param[inout] src the list whose elements will be tranferred to \c
 */
void ct_list_full_transfer(ct_list_o* restrict dst, ct_list_o* restrict src);

/**
 * Check if the given list is empty
 *
 * @param[in] l the list to analyze
 * @return
 * 	\li @true if \c l is empty;
 * 	\li @false otherwises
 */
bool ct_list_is_empty(const ct_list_o* l);

/**
 * Get and remove the head from the list \c l
 *
 * @param[inout] l the involved list
 * @return
 * 	\li The payload at the head of the list;
 * 	\li @null Wheter the list is empty;
 */
void* ct_list_pop(ct_list_o* l);

/**
 * Fetch the head of the list
 *
 * @param[in] l the list to handle
 * @return
 * 	\li the head of the list;
 * 	\li NULL if \c l is empty;
 */
void* ct_list_head(const ct_list_o* l);

/**
 * Fetch the tail of the list
 *
 * @param[in] l the list to analyze
 * @return
 * 	\li the tail of the list;
 * 	\li @null if \c l is empty
 */
void* ct_list_tail(const ct_list_o* l);

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
 * 	\li @null if the length of \c list is smaller than \c index itself;
 */
void* ct_list_get(const ct_list_o* list, int index);

/**
 *
 * \attention
 * You should use this function only inside ::VARIABLE_ITERATE_ON_LIST. Other uses may (and often will) lead to **undefined behaviour**!
 *
 * This function will allow you to dynamically remove elements inside a list when you're iterating it.
 * A typical example might be:
 *
 * @code
 * VARIABLE_ITERATE_ON_LIST(&list, previous, entry, payload) {
 * 	if (condition_to_payload()) {
 * 		ct_list_remove_entry_dynamic(&list, &previous, entry);
 * 	}
 * }
 * @endcode
 *
 * @param[in] l the list to change
 * @param[inout] previous_entry the entry that is positioned just before the one to remove. After this, \c previous_entry pointer will contain @null
 * @param[in] entry_to_remove the entry you want to remove
 *
 */
void ct_list_remove_entry_dynamic(ct_list_o* l, ct_list_entry_o** previous_entry, ct_list_entry_o* entry_to_remove);

/**
 * Get the next entry in the list.
 *
 * @param[in] entry the current element of the list
 * @return a entry containing the next element of the list or @null if we are currently on the tail of the list
 */
ct_list_entry_o* _ct_list_get_next_entry(const ct_list_entry_o* entry);

/**
 * Get the payload of the given list entry
 *
 * @param[in] entry the entry to handle
 * @return the payload within the entry. @null values doesn't necessary mean there is no attached value in the entry: for istance, a list of integer may
 * have such values representing a simple 0.
 */
void* _ct_list_get_entry_payload(const ct_list_entry_o* entry);

/**
 * Get the first entry of the list
 *
 * @param[in] l the list to handle
 * @return the first entry in the list
 */
ct_list_entry_o* _ct_list_head_entry(const ct_list_o* l);

/**
 * Allows you to transparently iterate through a list
 *
 * This macro is *slightly* faster than ::VARIABLE_ITERATE_ON_LIST but it doesn't allow you to
 * safely remove entries while iterating
 *
 * @param[in] _l a double point to the list you want to iterate through;
 * @param[in] entry name of the variable that will represent the entry under analysis
 * @param[in] _payload name of the variable representing <tt>entry->payload</tt>
 * @param[in] type type of the variable _payload
 */
#define ITERATE_ON_LIST(_l, entry, _payload, type) 												\
		const ct_list_o* UV(l) = (_l);															\
		ct_list_entry_o* entry = _ct_list_head_entry(UV(l)); 									\
		ct_list_entry_o* UV(next) = NULL;														\
		type _payload = NULL;																	\
		if (entry != NULL) {																	\
			_payload = _ct_list_get_entry_payload(entry);										\
			if (_ct_list_get_next_entry(entry) != NULL) {										\
				UV(next) = _ct_list_get_next_entry(entry);										\
			}																					\
		}																						\
		for (																					\
				;																				\
				entry != NULL;																	\
				entry = UV(next), 																\
				UV(next) = (entry != NULL) ? _ct_list_get_next_entry(entry) : NULL, 			\
				_payload = (entry != NULL) ? _ct_list_get_entry_payload(entry) : NULL			\
		)

/**
 * Allows you to transparently iterate through a list
 *
 * This macro is *slightly* slower than ::ITERATE_ON_LIST but allows you to
 * safely remove ct_list_entry_o while iterating.
 *
 * A typical example might be:
 *
 * @code
 * VARIABLE_ITERATE_ON_LIST(&list, previous, entry, payload) {
 * 	if (conditionToPayload()) {
 * 		ct_list_remove_entry_dynamic(&list, &previous, entry);
 * 	}
 * }
 * @endcode
 *
 * @param[in] _l a double point to the ct_list_o you want to iterate through;
 * @param[in] previous_entry name of the variable that will represent the entry just analyzed.
 * @param[in] entry name of the variable that will represent the entry under analysis
 * @param[in] _payload name of the variable representing <tt>entry->payload</tt>
 * @param[in] type type of the variable _payload
 */
#define VARIABLE_ITERATE_ON_LIST(_l, previous_entry, entry, _payload, type) 						\
		ct_list_o* UV(l) = (_l);																\
		ct_list_entry_o* UV(next) = NULL;														\
		type _payload = NULL;																	\
		if (_ct_list_head_entry(UV(l)) != NULL) {												\
			_payload = _ct_list_get_entry_payload(_ct_list_head_entry(UV(l)));					\
			if (_ct_list_get_next_entry(_ct_list_head_entry(UV(l))) != NULL) {					\
				UV(next) = _ct_list_get_next_entry(_ct_list_head_entry(UV(l)));					\
			}																					\
		}																						\
		for (																					\
				ct_list_entry_o 																\
				*previous_cell = NULL, 															\
				*UV(previous_cell_tmp) = NULL, 													\
				*entry = _ct_list_head_entry(UV(l)) 											\
				; 																				\
				entry != NULL 																	\
				; 																				\
				UV(previous_cell_tmp) = previous_cell == NULL ? UV(previous_cell_tmp) : entry, 	\
				previous_cell = UV(previous_cell_tmp), 											\
				entry = UV(next), 																\
				UV(next) = (entry != NULL) ? _ct_list_get_next_entry(entry) : NULL, 			\
				_payload = (entry != NULL) ? _ct_list_get_entry_payload(entry) : NULL			\
		)


#endif /* LIST_H_ */
