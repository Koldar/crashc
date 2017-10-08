/*
 * tag.c
 *
 *  Created on: Feb 16, 2017
 *      Author: koldar
 */

#include "tag.h"
#include <stdbool.h>
#include "hashtable.h"
#include "errors.h"

tag* initTag(const char* name) {
	tag* retVal = malloc(sizeof(tag));
	if (retVal == NULL) {
		MALLOCERRORCALLBACK();
	}

	retVal->name = strdup(name);

	return retVal;
}

void destroyTag(tag* tag) {
	free(tag->name);
	free(tag);
}

int compareTags(const tag* tag1, const tag* tag2) {
	if (tag1 == tag2) {
		return 0;
	}
	//TODO perform comparation(if the user wants it) with case insensitive as well!
	return strcmp(tag1->name, tag2->name);
}

bool haveTagSetsIntersection(const tag_ht* tagSet1, const tag_ht* tagSet2) {
	//TODO optimization: pick up the set with the least number of elements first. In this way the outer loop will be executed less

	ITERATE_VALUES_ON_HT(tagSet1, tag1, tag*) {
		ITERATE_VALUES_ON_HT(tagSet2, tag2, tag*) {
			if (compareTags(tag1, tag2) == 0) {
				return true;
			}
		}
	}

	return false;
}

void addTagNameInTagHashTable(tag_ht* tagHashTable, const char* name) {
	//TODO to improve performances you don't need to create a tag every time you see a name, There should be a tag pool containing
	//all the tags in the project. In this way, tag with the same name are created only once
	addItemInHTWithKey(tagHashTable, getHashOfString(name), initTag(name));
}

int getHashOfString(const char* str) {
	unsigned int hash = 5381;
	int c;

	while ((c = *str++))
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	return hash;
}

