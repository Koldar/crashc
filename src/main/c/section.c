/*
 * section.c
 *
 *  Created on: Feb 16, 2017
 *      Author: koldar
 */

#include "section.h"

static void printSectionData(const Section* section, bool recursive);


/** Adds the given section as a children to the specified parent section.
 *  The function also automatically reorganize the internal sections tree
 *  by properly updating the involved nodes pointers.
 *  Returns a pointer to the added section.
 */
Section* addSectionToParent(Section* restrict toAdd, Section* restrict parent) {
	//NEVER USED - Section* r = NULL;
	Section* list = NULL;

	toAdd->parent = parent;

	list = parent->firstChild;
	if (list == NULL) {
		parent->firstChild = toAdd;
		return toAdd;
	}
	while (true) {
		if (list->nextSibling != NULL) {
			list = list->nextSibling;
		} else {
			list->nextSibling = toAdd;
			return toAdd;
		}
	}
}

/** Returns the n-th child of the given section or NULL if it has none.
 */
Section* getNSection(const Section* parent, int nChild) {
	Section* list = parent->firstChild;
	while(true) {
		if ((nChild == 0) || (list == NULL)) {
			return list;
		} else {
			list = list->nextSibling;
			nChild--;
		}
	}
}

/** Initializes a new Section struct given its levelId, description and tags.
 *  Other values are initialized to their default.
 *  Returns a pointes to the newly created Section.
 */
Section* initSection(SectionLevelId levelId, const char* description, const char* tags) {
	Section* retVal = malloc(sizeof(Section));
	if (retVal == NULL) {
		MALLOCERRORCALLBACK();
	}

	retVal->accessGranted = false;
	retVal->alreadyFoundWhen = false;
	retVal->assertionReportList = initList();
	retVal->childrenNumber = 0;
	retVal->childrenNumberComputed = false;
	retVal->status = SECTION_UNEXEC;
	retVal->currentChild = 0;
	retVal->description = strdup(description);
	retVal->firstChild = NULL;
	retVal->failureReportList = initList();
	retVal->levelId = levelId;
	retVal->loopId = 0;
	//retVal->sectionToRunList = initList();
	retVal->loop1 = false;
	retVal->loop2 = false;
	retVal->nextSibling = NULL;
	retVal->parent = NULL;
	retVal->tags = NULL;

	populateTagsHT(retVal, tags, ' ');

	return retVal;
}

void destroySection(Section* section) {
	if (section->firstChild != NULL) {
		destroySection(section->firstChild);
	}
	if (section->nextSibling != NULL) {
		destroySection(section->nextSibling);
	}
	if (section->levelId <= 0) {
		//root section shouldn't be removed at all
		return;
	}

	Tag* current;
	Tag* tmp;
	HASH_ITER(hh, section->tags, current, tmp) {
		HASH_DEL(section->tags, current);
		destroyTag(current);
	}
	destroyList(section->failureReportList);
	//destroyListWithElement(section->sectionToRunList, destroySection);
	destroyListWithElement(section->assertionReportList, destroyTestReport);
	free(section->description);
	free(section);
}

/** Checks whether we are still computing the number of a section's children and returns
 *  true in this case, false otherwise.
 */
bool areWeComputingChildren(const Section* section) {
	return !section->childrenNumberComputed;
}

bool haveWeRunEverythingInSection(const Section* section) {
	return (section->status == SECTION_DONE);
}

bool haveWeRunEveryChildrenInSection(Section* section) {
	if (areWeComputingChildren(section)) {
		return false;
	}
	if (section->firstChild == NULL) {
		return true;
	}

	if (!haveWeRunEverythingInSection(section->firstChild)) {
		return false;
	}

	Section* tmp = section->firstChild->nextSibling;
	while (tmp != NULL) {
		if (!haveWeRunEverythingInSection(tmp)) {
			return false;
		}
		tmp = tmp->nextSibling;
	}
	return true;
}

/** Marks the given section as already executed by setting
 *  the struct appropriate boolean flag.
 */
void markSectionAsExecuted(Section* section) {
	section->status = SECTION_EXEC;
}

/*
 * We use this function to determine wheter we can set a section as
 * fully visited, thus we don't need to execute it anymore.
 * A section is considered fully executed in two cases:
 * 1- When it has no child
 * 2- When every child of the section is fully visited itself
 *
 * Note that we first check if the section has been executed at least once, as
 * if a given section has never been executed it surely can not be fully visited.
 */
bool isSectionFullyVisited(Section * section) {
	if (section->status == SECTION_UNEXEC) {
		return false;
	}

	if (section->childrenNumber == 0) {
		return true;
	}
	else {
		Section * next_child = section->firstChild;

		while (next_child != NULL) {
			if (!isSectionFullyVisited(next_child)) {
				return false;
			}
			next_child = next_child->nextSibling;
		}
		return true;
	}
}

void populateTagsHT(Section* section, const char* tags, char separator) {
	char token[100];
	char* positionToWriteInBuffer = NULL;
	int tokenId;
	Tag* tag;

	//i don't want to use strtok because it may be used inside the functions to test: i don't want to mess with their strtok starte
	while(*tags != '\0') {

		//fetch a tag inside tags
		positionToWriteInBuffer = token;
		while((*tags != separator) && (*tags != '\0')) {
			*positionToWriteInBuffer = *tags;
			positionToWriteInBuffer++;
			tags++;
		}
		*positionToWriteInBuffer = '\0';
		if (*tags != '\0') {
			//if we've not reached the end we need to go to the next tag
			tags++;
		}

		tokenId = hash(token);
		HASH_FIND_INT(section->tags, &tokenId, tag);
		if (tag == NULL) {
			tag = malloc(sizeof(Tag));
			if (tag == NULL) {
				MALLOCERRORCALLBACK();
			}
			tag->id = tokenId;
			tag->name = strdup(token);
			HASH_ADD_INT(section->tags, id, tag);
		}
	}
}
