/*
 * tag.h
 *
 *  Created on: Feb 16, 2017
 *      Author: koldar
 */

#ifndef TAG_H_
#define TAG_H_

#include <stdbool.h>

#include "hashtable.h"

/**
 * represents a single tag
 */
typedef struct tag {
	///the value of the hashtable
	char* name;
} tag;

/**
 * An alias to improve readability inside the code
 */
typedef HT tag_ht;

/**
 * Initialize a new tag
 *
 * @param[in] the tag name
 * @return the instance of the new tag in the heap
 */
tag* initTag(const char* name);

/**
 * Removed from memory a previously created tag
 *
 * @param[in] tag the tag to destroy
 */
void destroyTag(tag* tag);

/**
 * Compare 2 tags
 *
 * @param[in] tag1 the first tag
 * @param[in] tag2 the second tag
 * @return
 * 	\li \f$ ret < 0 \f$ if the name of the first tag is less than  the name of the second tag (with \c strcmp);
 * 	\li 0 if the 2 tags are the same (either by pointer or by tag names);
 * 	\li \f$ ret > 0 \f$ if the name of the first tag is greater than  the name of the second tag (with \c strcmp);
 */
int compareTags(const tag* tag1, const tag* tag2);

/**
 * Check if 2 tag sets have an intersection
 *
 * 2 tag hashtables have an intersection if at least one tag is present both in \c tagSet1 and in \c tagSet2
 *
 * @param[in] tagSet1 the first tagset to compute
 * @param[in] tagSet2 the second tagset to compute
 * @return
 * 	\li true if the 2 tagsets have somethin in common;
 * 	\li fasel otherwise;
 */
bool haveTagSetsIntersection(const tag_ht* tagSet1, const tag_ht* tagSet2);

/**
 * Adds a new tag inside a hashtable
 *
 * \post
 * 	\li after this operation, the hastable has increased its size by 1
 *
 * @param[inout] tagHashTable hashtable to populate
 * @param[in] name name of the tag to add
 */
void addTagNameInTagHashTable(tag_ht* tagHashTable, const char* name);

/**
 *
 * @param[in] str the string whose hash we want to compute
 * @return the hashcode of a string
 */
int getHashOfString(const char* str);

/**
 * Fetch the next token in the stream of characters representing a tag
 *
 * @param[in] str the string where we need to fetch the next token
 * @param[in] separator a character representing when a token ends and when it starts
 * @param[in] charactersToIgnore a string containing a list of characters we can safely ignore in \c str
 * @param[inout] output a buffer that will contain the token just read
 * @return the first character of rhe next new token or 0 if we reached the end of the string
 */
const char* computeNextTagInStr(const char* const str, char separator, char* charactersToIgnore, char* output);

/**
 * adds in \c output all the tags inside the stream of tags
 *
 * \post
 * 	\li \c output size increased
 *
 * @param[inout] output the hashtable where to add every tag found in \c tags
 * @param[in] tags a string containing tags, each of them separated by \c separator
 * @param[in] separator a character separating 2 tags. No double separators allwoed
 */
void populateTagsHT(tag_ht* output, const char* const tags, char separator);

#endif /* TAG_H_ */
