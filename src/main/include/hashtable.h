/**
 * \file hashtable.h
 *
 * Implements hashtables
 *
 * This module is just a front end to uthash
 *
 * Here's an example on how to use the hashtable. Of course you can exploit all macro available in \b ut_hash to find/loop and so on
 * even here. Those macros are explained <a href="https://troydhanson.github.io/uthash/userguide.html#_hash_operations">here</a>
 *
 * ```
 * HT* ht = initHT();
 * double* d = NULL;
 *
 * d = malloc(sizeof(double)); *d = 5;
 * addItemInHTWithKey(&ht,1, d);
 * d = malloc(sizeof(double)); *d = 127;
 * addItemInHTWithKey(&ht,5, d);
 * d = malloc(sizeof(double)); *d = -4.5;
 * addItemInHTWithKey(&ht,7, d);
 *
 * destroyHTWithElements(&ht, free);
 * ```
 *
 *  Created on: Jan 24, 2017
 *      Author: koldar
 */

#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include <stdbool.h>
#include "typedefs.h"
#include "uthash.h"
#include "macros.h"

/**
 * A structure representing a cell of the hash table
 *
 * see <a href="http://troydhanson.github.io/uthash/userguide.html">uthash</a> for further information
 */
typedef struct HTCell {
	unsigned long id;
	void* data;
	UT_hash_handle hh;
} HTCell;

typedef struct HT {
	HTCell* head;
} HT;

/**
 * Create a new hashtable in memory
 *
 * @return the new hashtable just created
 */
HT* initHT();

/**
 * \note
 * This operation is a O(1)
 *
 * @param[in] ht the hash table involved
 * @return the number of elements inside the hash table
 */
int getSizeOfHT(const HT* ht);

/**
 * get an element in the hashtable, given a certain key
 *
 * @param[in] ht the hashtable whwere we want to look int
 * @param[in] key the key associated to a certain element
 * @return
 * 	\li the element whose key is \c key;
 * 	\li NULL if it isn't present in the hashtable;
 */
void* getItemInHT(const HT* ht, unsigned long key);

/**
 * @param[in] ht the ht involved
 * @param[in] key the key involved
 * @return
 * 	\li true if there is a value within the hashtable whoe key is \c key;
 * 	\li false otheriwse
 */
bool containsItemInHT(const HT* ht, unsigned long key);

/**
 * Insert or alter an element inside the hash table
 *
 * If the hashtable doesn't have an element indexed with \c key, a new element will be created.
 * Otherwise we will update the existing element
 *
 * \attention
 * After this operation the old data will be totally overwritten! Be sure to still have a backup reference
 * of the previous object, otherwise memory leak will likely to happen!
 *
 * @param[in] ht the hashtable to analyze
 * @param[in] key the key of the element to update
 * @param[in] data the new data to overwrite the old one
 * @return
 * 	\li true if a new element is created;
 * 	\li false if we overwrote the previous one
 */
bool addOrUpdateItemInHT(HT* ht, unsigned long key, void* data);

/**
 * Updates the value indexed by \c key to a new value
 *
 * \note
 * If there is no element indexed with \c key in the hashtable, this function does nothing
 *
 * @param[in] ht the hashtable to alter
 * @param[in] key the key referring to the element to update
 * @param[in] data the new data that will replace the old one
 * @return
 * 	\li true if we have update with success the data;
 * 	\li false if we couldn't find any cell indexed with \c key;
 */
bool updateItemInHT(HT* ht, unsigned long key, void* data);

void addItemInHTWithKey(HT* ht, unsigned long key, void* data);

void destroyHT(HT* ht);

void destroyHTWithElements(HT* ht, ct_destructor_t d);

/**
 * Delete a cell inside the hashtable
 *
 * \post
 * 	\li don't use \c htCell after this call because it will cause an invalid read
 *
 * @param[in] ht the hash table to change
 * @param[in] htCell the cell to remove
 */
void deleteHTCell(HT* ht, HTCell* htCell);

/**
 * like ::destroyHTCell but it delete from the memory also the element inside the cell
 *
 * @param[in] htCell the cell to remove from the memory
 * @param[in] d the destructor to use to remove the data inside the \c htCell
 */
void destroyHTCellWithElement(HTCell* htCell, ct_destructor_t d);

bool deleteItemInHT(HT* ht, unsigned long key);

bool deleteItemInHTWithElement(HT* ht, unsigned long key, ct_destructor_t d);

bool isHTEmpty(const HT* ht);

/**
 * Fetch the first item the program can find within the hashtable.
 *
 * Nothing is said about what the softwar epicks up: <b>don't assume it was the first one you have added in the hashtable!</b>
 *
 * @param[in] ht the ht invovled
 * @return
 * 	\li an item inside the \c ht;
 * 	\li NULL if the \c ht is empty;
 */
void* getFirstItemInHT(const HT* ht);

/**
 * Swap the contents of the values indexed by \c key1 and \c keys
 *
 * If one key does not exist in the hashtable, after this operation it will exist whilst the
 * former one will be removed
 *
 * @param[in] ht the ht to handle
 * @param[in] key1 the first key
 * @param[in] key2 the second key
 * @return
 * 	\li true if a swap is performed;
 * 	\li false if both keys are not present;
 */
bool swapValuesInHT(HT* ht, unsigned long key1, unsigned long key2);

/**
 * Clear all the elements inside the given hashtable.
 *
 * The values inside the hashtable won't be touched; however make sure
 * you have other reference of them otherwise memory leak might happen
 *
 * @param[inout] ht the hashtable to clear
 */
void clearHT(HT* ht);

/**
 * Clear all the elements inside the given hashtable
 *
 * Contrarly to ::clearHT, this function will release the memory of the paylaod inside the hashtable as well!
 *
 * \post
 * 	\li hashtable empty;
 * 	\li elements in hashtable released from memory
 *
 * @param[inout] ht the hashtable to clear
 * @param[in] d the function to use to destroy the paylaod of the hashtable
 */
void clearHTWithElements(HT* ht, ct_destructor_t d);

//#define ITERATE_ON_HT(ht, pair) \
//	HT** UV(_ht) = *(ht);\
//	HTCell* UV(tmp);\
//	HASH_ITER(hh, UV(_ht), pair, UV(tmp))

//Picked up from uthash HASH_ITER Definition
#define ITERATE_ON_HT(_head,el)   \
	HTCell* UV(head) = ((HT*)(_head))->head;\
	HTCell* el = NULL; \
	HTCell* UV(tmp) = NULL;\
	for(((el)=(UV(head))), ((*(char**)(&(UV(tmp))))=(char*)((UV(head)!=NULL)?(UV(head))->hh.next:NULL)); \
  (el) != NULL; ((el)=(UV(tmp))), ((*(char**)(&(UV(tmp))))=(char*)((UV(tmp)!=NULL)?(UV(tmp))->hh.next:NULL)))

#ifdef NO_DECLTYPE
#define HASH_ITER(hh,head,el,tmp)                                                \
for(((el)=(head)), ((*(char**)(&(tmp)))=(char*)((head!=NULL)?(head)->hh.next:NULL)); \
  (el) != NULL; ((el)=(tmp)), ((*(char**)(&(tmp)))=(char*)((tmp!=NULL)?(tmp)->hh.next:NULL)))
#else
#define HASH_ITER(hh,head,el,tmp)                                                \
for(((el)=(head)), ((tmp)=DECLTYPE(el)((head!=NULL)?(head)->hh.next:NULL));      \
  (el) != NULL; ((el)=(tmp)), ((tmp)=DECLTYPE(el)((tmp!=NULL)?(tmp)->hh.next:NULL)))
#endif

/**
 * Macro allowing you to go through every element of hashtable
 *
 * An example of usage might be:
 *
 * <pre><code>
 * HT* ht;
 * ITERATE_VALUES_ON_HT(&ht, el, int) {
 * 	printf("value %d\n", *el);
 * }
 * </code></pre>
 *
 * @param[in] _head double point to an hashtable to go through
 * @param[in] _data the name of the variable that will contain a value in the iteration
 * @param[in] type the type of \c _data. So if you put \c int, data will have type <tt>int</tt>
 */
#define ITERATE_VALUES_ON_HT(_head,_data,type) \
	HTCell* UV(head) = ((HT*)(_head))->head;\
	HTCell* UV(el) = NULL; \
	HTCell* UV(tmp) = NULL;\
	type _data = NULL; \
	if (UV(head) != NULL) { \
		_data = UV(head)->data; \
	} \
	for(\
		(UV(el)=(UV(head))), \
		((*(char**)(&(UV(tmp))))=(char*)((UV(head) != NULL)?(UV(head))->hh.next:NULL)) \
		; \
		UV(el) != NULL \
		; \
		(UV(el)=(UV(tmp))), \
		_data=(UV(el) != NULL) ? UV(el)->data : NULL,\
		((*(char**)(&(UV(tmp))))=(char*)((UV(tmp)!=NULL)?(UV(tmp))->hh.next:NULL))\
	)


#endif /* HASHTABLE_H_ */
