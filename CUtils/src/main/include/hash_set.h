/*
 * \file hash_set.h
 *
 * An hash set is an implementation of a set.
 *
 * A set is an unordered group of elements. You can check if an element belong to the set in O(1).
 * This module implement the set via an hashtable
 *
 *  Created on: Dec 21, 2017
 *      Author: koldar
 */

#ifndef HASH_SET_H_
#define HASH_SET_H_

#include <stdbool.h>
#include "hashtable.h"
#include "payload_functions.h"

typedef struct hash_set hash_set;

/**
 * A set containining, instead of \c void*, the actual int paylaod
 */
typedef hash_set pint_hash_set;
/**
 * A set containing strings
 */
typedef hash_set string_hash_set;

/**
 * Initialize an hash set
 *
 * @param[in] functions support function to manage the payload of this container
 */
hash_set* initHashSet(payload_functions functions);

/**
 * Adds an item inside the set
 *
 * @param[inout] set the set where to add the item
 * @param[in] item the item to add
 * @return
 * 	\li true if we have added \c item in the set;
 * 	\li false if \c item was already inside the set;
 */
bool addItemInHashSet(hash_set* set, const void* item);

/**
 * Check if \c item is inside the set
 *
 * @param[in] set the set to manage
 * @param[in] item the item to check
 * @return
 * 	\li true if \c item is inside \c set;
 * 	\li false otherwise;
 */
bool containsItemInHashSet(const hash_set* set, const void* item);

/**
 * Remove an item inside the set
 *
 * \note
 * The function does nothing if \c item is not inside the set from the beginning
 *
 * @param[inout] set the set to manage
 * @param[in] item the element to remove from the set
 */
void removeItemInHashSet(hash_set* set, const void* item);

/**
 * Release the memory from this set
 *
 * \note
 * Paylaod won't be release from the memory at all
 *
 * @param[in] set the set to release from memory;
 */
void destroyHashSet(hash_set* set);

/**
 * Release the memory from this set
 *
 * \note
 * The function will remove from the memory the payload as well!
 *
 * @param[in] set the set to release from memory;
 */
void destroyHashSetWithElements(hash_set* set);

/**
 * the size of the set
 *
 * @param[in] set the set to manage
 * @return the size of the set
 */
size_t getSizeOfHashSet(const hash_set* set);

/**
 * true if the set is empty
 *
 * @param[in] set the set to manage
 * @return
 * 	\li true if the set has no items within it;
 * 	\li false otherwise
 */
bool isHashSetEmpty(const hash_set* set);

/**
 * Compute the \f$ set_{1} \cup set_{2} \f$
 *
 * \note
 * \c set1 and \c set2 can't be the same!
 *
 * @param[inout] set1 the first set to consider. If \c inPlace is true, this set **will be modified!**
 * @param[in] set2 the second set to consider;
 * @param[in] inPlace if false, the 2 sets won't be changed and the result will be the output. If \c inPlace is true, the return
 * 	value will be the same reference of \c set1, and \c set1 variable itself will be changed
 * @return the union of the 2 sets. **Payloads are copied by reference**.
 */
hash_set* getUnionOfHashSets(hash_set* restrict set1, const hash_set* restrict set2, bool inPlace);

/**
 * Compute the \f$ set_{1} \cap set_{2} \f$
 *
 * \note
 * \c set1 and \c set2 can't be the same!
 *
 * @param[inout] set1 the first set to consider. If \c inPlace is true, this set **will be modified!**
 * @param[in] set2 the second set to consider;
 * @param[in] inPlace if false, the 2 sets won't be changed and the result will be the output. If \c inPlace is true, the return
 * 	value will be the same reference of \c set1, and \c set1 variable itself will be changed
 * @return the intersection of the 2 sets. **Payloads are copied by reference**.
 */
hash_set* getIntersectionOfHashSets(hash_set* restrict set1, const hash_set* restrict set2, bool inPlace);

/**
 * Compute the \f$ set_{1} / set_{2} \f$
 *
 * \note
 * \c set1 and \c set2 can't be the same!
 *
 * @param[inout] set1 the first set to consider. If \c inPlace is true, this set **will be modified!**
 * @param[in] set2 the second set to consider;
 * @param[in] inPlace if false, the 2 sets won't be changed and the result will be the output. If \c inPlace is true, the return
 * 	value will be the same reference of \c set1, and \c set1 variable itself will be changed
 * @return the difference of the 2 sets. **Payloads are copied by reference**.
 */
hash_set* getDifferenceOfHashSets(hash_set* restrict set1, const hash_set* restrict set2, bool inPlace);

/**
 * Empty the hash set
 *
 * @param[in] set the set to manage
 */
void clearHashSet(const hash_set* set);

/**
 * Empty the hash set
 *
 * \note
 * this function will release from the memory all the items as well!
 *
 * @param[in] set the set to manage
 */
void clearHashSetWithElements(const hash_set* set);

/**
 * compute a string representation of the set
 *
 * @param[in] set the set to manage
 * @param[inout] buffer the buffer where to store the string representation
 * @return the number of bytes which have been filled in \c buffer
 */
int getBufferStringOfHashSet(const hash_set* set, char* buffer);

/**
 * Clone the set by copy the payloads within the set by reference
 *
 * This means that no deep copy is performed; however now the same data is accessible through 2 different sets;
 *
 * @param[in] set the set to manage
 */
hash_set* cloneHashSetByReference(const hash_set* set);

/**
 * Clone the set by deep copying it
 *
 * This mean a **deep copy** is performed (depending on ::payload_functions implementation)
 *
 * @param[in] set the set to manage
 */
hash_set* cloneHashSet(const hash_set* set);

/**
 * Get the hashtable under this set implementation
 *
 * \note
 * protected function. Shouldn't be used at all
 *
 * @param[in] set the set to manage
 * @return the hash table representing the set
 */
HT** _getUnderlyingHashTableInHashSet(const hash_set* set);

/**
 * Macro to comfortably loop over the values inside the set
 *
 * @code
 * hash_set set = initHashSet(...);
 * //add some values in "set"
 * ITERATE_OVER_HASHSET(set, var, double) {
 * 	//var will be of type "double*"
 * 	printf("the set contains %f\n", *var);
 * }
 * @endcode
 *
 * \note
 * the ordering of iteration is **not garantueed to be deterministic**.
 *
 * @param[in] aset the set to loop over
 * @param[in] name the name of the variable which will contain the value to iterate over
 * @param[in] type the type the variable \c name will have within the cycle. It will always be a pointer of \c type.
 */
#define ITERATE_OVER_HASHSET(aset, name, type) \
	for (bool UV(setloop)=true; UV(setloop); ) \
		for (hash_set* UV(set)=(aset); UV(setloop); ) \
			for (HT** UV(ht)=_getUnderlyingHashTableInHashSet(UV(set)); UV(setloop); UV(setloop)=false) \
				CORRECT_ITERATE_VALUES_ON_HT(UV(ht), name, type)

#define VARIABLE_ITERATE_OVER_HASHSET(aset, name, type) \
	for (bool UV(setloop)=true; UV(setloop); ) \
		for (hash_set* UV(set)=(aset); UV(setloop); ) \
			for (HT** UV(ht)=_getUnderlyingHashTableInHashSet(UV(set)); UV(setloop); UV(setloop)=false) \
				CORRECT_ITERATE_VALUES_ON_HT(UV(ht), name, type)


#endif /* HASH_SET_H_ */
