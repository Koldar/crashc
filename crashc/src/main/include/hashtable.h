/**
 * @file
 *
 * Implements hashtables
 *
 * This module is just a front-end to uthash
 *
 * Here's an example on how to use the hashtable. Of course you can exploit all the macros available in \b ut_hash to find/loop and so on
 * even here. Those macros are explained <a href="https://troydhanson.github.io/uthash/userguide.html#_hash_operations">here</a>
 *
 * ```
 * ct_hashtable_o* ht = ct_ht_init();
 * double* d = NULL;
 *
 * d = malloc(sizeof(double)); *d = 5;
 * ct_ht_put(&ht,1, d);
 * d = malloc(sizeof(double)); *d = 127;
 * ct_ht_put(&ht,5, d);
 * d = malloc(sizeof(double)); *d = -4.5;
 * ct_ht_put(&ht,7, d);
 *
 * ct_ht_destroy_with_elements(&ht, free);
 * ```
 *
 * @author koldar
 * @date Jan 24, 2017
 */

#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include <stdbool.h>
#include <stdlib.h> /* For NULL macro */

#include "typedefs.h"
#include "uthash.h"
#include "macros.h"

/**
 * A structure representing an entry of the hash table
 *
 * see <a href="http://troydhanson.github.io/uthash/userguide.html">uthash</a> for further information
 */
struct ct_hashtable_entry;
typedef struct ct_hashtable_entry ct_hashtable_entry_o;

/**
 * Structure representing the facet of the hashtable data structure
 */
struct ct_hashtable;
typedef struct ct_hashtable ct_hashtable_o;

/**
 * Create a new hashtable in memory
 *
 * @return the new hashtable just created
 */
ct_hashtable_o* ct_ht_init();

/**
 * number of item in the hashtable
 *
 * \note
 * This operation is O(1)
 *
 * @param[in] ht the hash table involved
 * @return the number of elements inside the hash table
 */
int ct_ht_size(const ct_hashtable_o* ht);

/**
 * get an element in the hashtable, given a certain key
 *
 * @param[in] ht the hashtable whwere we want to look int
 * @param[in] key the key associated to a certain element
 * @return
 * 	\li the element whose key is \c key;
 * 	\li NULL if it isn't present in the hashtable;
 */
void* ct_ht_get(const ct_hashtable_o* ht, unsigned long key);

/**
 * Check if a value in the hashtable has a particular key
 *
 * @param[in] ht the ht involved
 * @param[in] key the key involved
 * @return
 * 	\li @true if there is a value within the hashtable whose key is \c key;
 * 	\li @false otheriwse
 */
bool ct_ht_contains(const ct_hashtable_o* ht, unsigned long key);

/**
 * Insert or alter an element inside the hash table
 *
 * If the hashtable doesn't have an element indexed with \c key, a new element will be created.
 * Otherwise we will update the existing element
 *
 * \attention
 * After this operation the old data will be totally overwritten! Be sure to still have a backup reference
 * of the previous object, otherwise memory leaks will likely happen!
 *
 * @param[inout] ht the hashtable to analyze
 * @param[in] key the key of the element to update
 * @param[in] data the new data to overwrite the old one
 * @return
 * 	\li @true if a new element is created;
 * 	\li @false if we overwrote the previous one
 */
bool ct_ht_put_or_update(ct_hashtable_o* ht, unsigned long key, const void* data);

/**
 * Updates the value indexed by \c key to a new value
 *
 * \note
 * If there is no element indexed with \c key in the hashtable, this function does nothing
 *
 * @param[inout] ht the hashtable to alter
 * @param[in] key the key referring to the element to update
 * @param[in] data the new data that will replace the old one
 * @return
 * 	\li @true if we have update with success the data;
 * 	\li @false if we couldn't find any cell indexed with \c key;
 */
bool ct_ht_update(ct_hashtable_o* ht, unsigned long key, const void* data);

/**
 * Insert a new value within the hashtable
 *
 * \note
 * Undefined behaviour if the key already exists in the hashtable
 *
 *
 * @param[inout] ht the hashtable to handle
 * @param[in] key the key of \c data
 * @param[in] data the actual value to store in the hastable
 */
void ct_ht_put(ct_hashtable_o* ht, unsigned long key, const void* data);

/**
 * Release the hashtable from the memory
 *
 * \note
 * If the values in the hastable are in memory as well, they won't be freed at all
 *
 * @param[inout] ht the hashtable to remove
 * @see ct_ht_destroy_with_elements
 */
void ct_ht_destroy(ct_hashtable_o* ht);

/**
 * Like ::ct_ht_destroy but it frees the items in the hashtable from memory as well
 *
 * @param[inout] ht the hashtable to remove
 * @param[in] d a function to use to free the elements in the hashtable from memory
 */
void ct_ht_destroy_with_elements(ct_hashtable_o* ht, ct_destroyer_c d);

/**
 * Remove an element from the hashtable
 *
 * \note
 * The element won't be removed from the memory at all
 *
 * @param[inout] ht the hashtable to handle
 * @param[in] key the key of the value to remove from the hashtable
 * @see ct_ht_remove_and_destroy
 */
bool ct_ht_remove(ct_hashtable_o* ht, unsigned long key);

/**
 * like ::ct_ht_remove but it frees the value from memory as well
 *
 * @param[inout] ht the hashtable to handle
 * @param[in] key the key of the value to remove from the hashtable
 * @param[in] d the function to use to dispose of the value removed from the hashtable
 */
bool ct_ht_remove_and_destroy(ct_hashtable_o* ht, unsigned long key, ct_destroyer_c d);

/**
 * checks if the hashtable contains no values
 *
 * @param[in] ht the hashtable to handle
 * @return @true if the hashtable contains no values; @false otherwise;
 */
bool ct_ht_is_empty(const ct_hashtable_o* ht);

/**
 * Fetch the first item the program can find within the hashtable.
 *
 * Nothing is said about what the software picks up: **don't assume it was the first one you have added in the hashtable!**
 *
 * @param[in] ht the hashtable involved
 * @return
 * 	\li an item inside the \c ht;
 * 	\li @null if the \c ht is empty;
 */
void* ct_ht_get_first(const ct_hashtable_o* ht);

/**
 * Swap the contents of the values indexed by \c key1 and \c keys
 *
 * If one key does not exist in the hashtable, after this operation it will exist whilst the
 * former one will be removed
 *
 * @param[inout] ht the ht to handle
 * @param[in] key1 the first key
 * @param[in] key2 the second key
 * @return
 * 	\li @true if a swap is performed;
 * 	\li @false if both keys are not present;
 */
bool ct_ht_swap(ct_hashtable_o* ht, unsigned long key1, unsigned long key2);

/**
 * Clear all the elements inside the given hashtable.
 *
 * The values inside the hashtable won't be touched; however make sure
 * you have other reference of them otherwise memory leak might happen
 *
 * @param[inout] ht the hashtable to clear
 */
void ct_ht_clear(ct_hashtable_o* ht);

/**
 * Clear all the elements inside the given hashtable
 *
 * Contrary to ::ct_ht_clear, this function will release the memory of the payloads inside the hashtable as well!
 *
 * \post
 * 	\li hashtable empty;
 * 	\li elements in hashtable released from memory
 *
 * @param[inout] ht the hashtable to clear
 * @param[in] d the function to use to destroy the paylaod of the hashtable
 */
void ct_ht_clear_and_destroy_elements(ct_hashtable_o* ht, ct_destroyer_c d);

/**
 * Returns the head entry of the hashtable.
 *
 * @param[inout] ht The hashtable on which to operate
 * @return The head entry of the hashtable
 */
ct_hashtable_entry_o* _ct_ht_head_entry(ct_hashtable_o* ht);

/**
 * Returns the value of the "next" field in the UT_hash_handle field of an entry.
 * \note
 * This function is purely instrumental towards hiding implementation details to the hashtable user
 *
 * @param[in] entry The hashtable entry on which the desired handle resides
 * @return A pointer to the next value contained in the hashtable, considered by insertion order
 */
void* _ct_ht_entry_next(ct_hashtable_entry_o* entry);

/**
 * Returns the payload of a given hashtable entry
 *
 * @param[in] entry The entry which contains the desired payload
 * @return The payload of the entry
 */
void* _ct_ht_entry_payload(ct_hashtable_entry_o* entry);

//Picked up from uthash HASH_ITER Definition
#ifdef CT_ITERATE_ON_HT
#	error "CrashC - CT_ITERATE_ON_HT macro already defined!"
#endif
#define CT_ITERATE_ON_HT(ht,el)   																								\
	ct_hashtable_entry_o* CT_UV(head) = _ct_ht_head_entry((ct_hashtable_o*) (ht));												\
	ct_hashtable_entry_o* el = NULL; 																						\
	ct_hashtable_entry_o* CT_UV(tmp) = NULL;																					\
	for(((el)=(CT_UV(head))), ((*(char**)(&(CT_UV(tmp))))=(char*)((CT_UV(head)!=NULL)?_ct_ht_entry_next(CT_UV(head)):NULL)); 			\
  (el) != NULL; ((el)=(CT_UV(tmp))), ((*(char**)(&(CT_UV(tmp))))=(char*)((CT_UV(tmp)!=NULL)?_ct_ht_entry_next(CT_UV(tmp)):NULL)))

//TODO replace this code with the correct code in Cutils!!!!
/**
 * Macro allowing you to go through every element of hashtable
 *
 * An example of usage might be:
 *
 * <pre><code>
 * ct_hashtable_o* ht;
 * CT_ITERATE_VALUES_ON_HT(&ht, el, int) {
 * 	printf("value %d\n", *el);
 * }
 * </code></pre>
 *
 * @param[in] ht Pointer to the hashtable on which to iterate
 * @param[in] _data the name of the variable that will contain a value in the iteration
 * @param[in] type the type of \c _data. So if you put \c int, data will have type <tt>int</tt>
 */
#ifdef CT_ITERATE_VALUES_ON_HT
#	error "CrashC - CT_ITERATE_VALUES_ON_HT macro already defined!"
#endif
#define CT_ITERATE_VALUES_ON_HT(ht,_data,type) 															\
	ct_hashtable_entry_o* CT_UV(head) = _ct_ht_head_entry((ct_hashtable_o*)(ht));						\
	ct_hashtable_entry_o* CT_UV(el) = NULL; 															\
	ct_hashtable_entry_o* CT_UV(tmp) = NULL;															\
	type _data = NULL; 																					\
	if (CT_UV(head) != NULL) { 																			\
		_data = _ct_ht_entry_payload(CT_UV(head)); 														\
	} 																									\
	for(																								\
		(CT_UV(el)=(CT_UV(head))), 																		\
		((*(char**)(&(CT_UV(tmp))))=(char*)((CT_UV(head) != NULL)?_ct_ht_entry_next(CT_UV(head)):NULL)) \
		; 																								\
		CT_UV(el) != NULL 																				\
		; 																								\
		(CT_UV(el)=(CT_UV(tmp))), 																		\
		_data=(CT_UV(el) != NULL) ? _ct_ht_entry_payload(CT_UV(el)) : NULL,								\
		((*(char**)(&(CT_UV(tmp))))=(char*)((CT_UV(tmp)!=NULL)?_ct_ht_entry_next(CT_UV(tmp)):NULL))		\
	)


#endif /* HASHTABLE_H_ */
