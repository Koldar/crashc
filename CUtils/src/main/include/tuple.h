/**
 * @file
 *
 * The module contains several types representing pair, triples and, more generally, tuples.
 *
 *  Created on: Jan 22, 2018
 *      Author: koldar
 */

#ifndef TUPLE_H_
#define TUPLE_H_

#include "typedefs.h"

/**
 * A pair of generic pointers.
 */
typedef struct {
	void* first;
	void* second;
} pointer_pair_t;

/**
 * Initialize a new pair of void pointers
 *
 * @param[in] first the first pointer to initialize the first field of the pair
 * @param[in] second the second pointer to initialize the second field of the pair
 * @return the pair of generic pointers
 */
pointer_pair_t* initPointerPair(const void* first, const void* second);

/**
 * Destroy from memory the pair of pointers.
 *
 * \note
 * Paylaod won't be released from memory though
 *
 * @param[in] p the pair to destroy
 */
void destroyPointerPair(const pointer_pair_t* p);

/**
 * like ::destroyPointerPair but it destory also the payload of the pair
 *
 * @param[in] p the pair to destroy
 * @param[in] dFirst the destructor of the first payload of the pair
 * @param[in] dSecond the destructor of the first payload of the pair
 */
void destroyPointerPairWithElements(const pointer_pair_t* p, destructor dFirst, destructor dSecond);

#endif /* TUPLE_H_ */
