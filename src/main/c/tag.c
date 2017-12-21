/*
 * tag.c
 *
 *  Created on: Feb 16, 2017
 *      Author: koldar
 */


#include <stdbool.h>

#include "tag.h"
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

const char* computeNextTagInStr(const char* const str, char separator, char* charactersToIgnore, char* output) {
	char* outputIndex = NULL;
	char ch = '\0';
	const char* input = NULL;
	bool toSkip = false;

	//I don't use strtok because I don't want to use something with side effects
	input = str;
	outputIndex = &output[0];
	while (true) {
		ch = *input;

		//check if the character has to be skipped
		toSkip = false;
		for (int i=0; (charactersToIgnore[i])!='\0'; i++) {
			if (ch == charactersToIgnore[i]) {
				toSkip = true;
			}
		}

		if (!toSkip) {
			//add the character to the buffer
			if (ch == separator || ch == '\0') {
				*outputIndex = '\0';
				goto exit;
			} else {
				*outputIndex = ch;
			}
		}

		//go to the next character
		outputIndex += 1;
		input += 1;
	}

	exit:;
	if (*input != '\0') {
		input++;
	}
	return input;
}

void populateTagsHT(tag_ht* output, const char* const tags, char separator) {
	char token[100];
	char* positionToWriteInBuffer = NULL;
	int tokenId;

	const char* tokenString = tags;

	while (*tokenString != '\0') {
		tokenString = computeNextTagInStr(tokenString, separator, "", token);

		if (strlen(token) == 0) {
			return;
		}

		//add the fetched tag inside the section
		tokenId = getHashOfString(token);
		tag* tagWithTokenId = getItemInHT(output, tokenId);
		if (tagWithTokenId == NULL) {
			tagWithTokenId = initTag(token);
			addItemInHTWithKey(output, tokenId, tagWithTokenId);
		}
	}
}
