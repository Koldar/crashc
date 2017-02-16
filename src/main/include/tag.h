/*
 * tag.h
 *
 *  Created on: Feb 16, 2017
 *      Author: koldar
 */

#ifndef TAG_H_
#define TAG_H_

#include "uthash.h"

/**
 * represents a single tag cell inside a ::TagHashTable
 */
typedef struct Tag {
	///the key of the hashtable
	int id;
	///the value of the hashtable
	char* name;
	UT_hash_handle hh;
} Tag;

/**
 * An alias to improve readability inside the code
 */
typedef Tag TagHashTable;

/**
 * Removed from memory a previously created tag
 *
 * @param[in] tag the tag to destroy
 */
void destroyTag(Tag* tag);


#endif /* TAG_H_ */
