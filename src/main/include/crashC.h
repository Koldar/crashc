#ifndef CRASHC_H_
#define CRASHC_H_

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "uthash.h"

typedef struct Tag {
	int id;
	char* name;
	UT_hash_handle hh;
} Tag;

typedef Tag TagHashTable;

typedef int SectionLevelId;
struct SectionCell;

typedef struct Section {
	SectionLevelId levelId;
	const char* description;
	TagHashTable* tags;

	bool childrenNumberComputed;
	int childrenNumber;
	bool executed;

	int currentChild;
	int loopId;

	bool loop1;
	bool loop2;
	bool accessGranted;

	//STRUCTURE USED ONLY FOR CONTAINABLE SECTION IMPLEMENTATIONS
	struct SectionCell* sectionToRunList;
	//END

	struct Section* parent;
	struct Section* firstChild;
	struct Section* nextSibling;
} Section;

typedef struct SectionCell {
	Section* section;
	struct SectionCell* next;
} SectionCell;

typedef bool (*condition_section)(Section*);
typedef void (*AfterExecutedSectionCallBack)(Section** parentPosition, Section* child);
typedef void (*BeforeStartingSectionCallBack)(Section* sectionGranted);

Section rootSection = {0, "root", false, 0, 0, false, NULL, NULL, NULL};
Section* currentSection = NULL;

#define MALLOCERRORCALLBACK()

SectionCell* initSectionList();
void addSectionInHeadSectionList(SectionCell** list, Section* section);
Section* peekSectionInHeadSectionList(SectionCell** list);
bool containsSectionInSectionList(SectionCell** list, Section* section);
bool removeSectionInSectionList(SectionCell** list, Section* section);
Section* popHeadSectionInSectionList(SectionCell** list);
void destroySectionList(SectionCell** list);
Section* addSectionToParent(Section* toAdd, Section* parent);
Section* getNSection(Section* parent, int nChild);
Section* initSection(SectionLevelId levelId, const char* description, const char* tags);
void destroySection(Section* section);
void printSectionData(Section* section, bool recursive);
bool areWeComputingChildren(Section* section);
bool runOnceAndCheckAccessToSection(Section* section, condition_section cs, BeforeStartingSectionCallBack callback);
bool runOnceAndDoWorkAtEnd(Section* section, Section** pointerToSetAsParent, AfterExecutedSectionCallBack callback, AfterExecutedSectionCallBack accessGrantedCallback, AfterExecutedSectionCallBack accessDeniedCallback);
bool haveWeRunEverythingInSection(Section* section);
bool haveWeRunEveryChildrenInSection(Section* section);
void markSectionAsExecuted(Section* section);
Section* getSectionOrCreateIfNotExist(Section* parent, SectionLevelId sectionLevelId, const char* decription, const char* tags);
void destroyTag(Tag* tag);

int hash(const char* str);
void populateTagsHT(Section* section, const char* tags, char separator);

bool getAlwaysTrue(Section* section);
bool getAccessOnlyIfNotInBlackList(Section* section);

SectionCell* initSectionList() {
	return NULL;
}

void addSectionInHeadSectionList(SectionCell** list, Section* section) {
	SectionCell* retVal = malloc(sizeof(SectionCell));
	if (retVal == NULL) {
		MALLOCERRORCALLBACK();
	}

	retVal->section = section;
	retVal->next = NULL;

	if ((*list) == NULL) {
		*list = retVal;
	} else {
		retVal->next = *list;
		*list = retVal;
	}
}

Section* peekSectionInHeadSectionList(SectionCell** list) {
	return (*list) != NULL ? (*list)->section : NULL;
}

void addSectionInTailSectionList(SectionCell** list, Section* section) {
	SectionCell* toAdd = malloc(sizeof(SectionCell));
	if (toAdd == NULL) {
		MALLOCERRORCALLBACK();
	}

	toAdd->section = section;
	toAdd->next = NULL;

	SectionCell* tmp = *list;
	SectionCell** position = list;
	while (tmp != NULL) {
		position = &tmp->next;
		tmp = tmp->next;
	}
	*position = toAdd;
}

bool removeSectionInSectionList(SectionCell** list, Section* section) {
	SectionCell* tmp = *list;
	SectionCell* previous = NULL;
	while (tmp != NULL) {
		if (tmp->section == section) {
			if (previous == NULL) {
				//first iteration
				*list = tmp->next;
			} else {
				previous->next = tmp->next;
			}
			free(tmp);
			return true;
		}
		previous = tmp;
		tmp = tmp->next;
	}
	return false;
}

Section* popHeadSectionInSectionList(SectionCell** list) {
	if (*list == NULL) {
		return NULL;
	}
	Section* retVal = (*list)->section;
	*list = (*list)->next;
	return retVal;
}

bool containsSectionInSectionList(SectionCell** list, Section* section) {
	SectionCell* tmp = *list;
	while (tmp != NULL) {
		if (tmp->section == section) {
			return true;
		}
		tmp = tmp->next;
	}
	return false;
}

void destroySectionList(SectionCell** list) {
	SectionCell* tmp = *list;
	SectionCell* tmp2 = NULL;
	while (tmp != NULL) {
		tmp2 = tmp->next;
		free(tmp);
		tmp = tmp2;
	}
	*list = NULL;
}

Section* addSectionToParent(Section* toAdd, Section* parent) {
	Section* r = NULL;
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

Section* getNSection(Section* parent, int nChild) {
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

void destroyTag(Tag* tag) {
	free(tag->name);
	free(tag);
}

Section* initSection(SectionLevelId levelId, const char* description, const char* tags) {
	Section* retVal = malloc(sizeof(Section));
	if (retVal == NULL) {
		MALLOCERRORCALLBACK();
	}

	retVal->accessGranted = false;
	retVal->childrenNumber = 0;
	retVal->childrenNumberComputed = false;
	retVal->executed = false;
	retVal->currentChild = 0;
	retVal->description = strdup(description);
	retVal->firstChild = NULL;
	retVal->levelId = levelId;
	retVal->loopId = 0;
	retVal->sectionToRunList = initSectionList();
	retVal->loop1 = false;
	retVal->nextSibling = NULL;
	retVal->parent = NULL;
	retVal->tags = NULL;

	populateTagsHT(retVal, tags, ' ');

	return retVal;
}

/**
 * Given a string \c tags, the function will populate the hash table in \c section with all
 * the tags inside \c tags
 *
 * For example if \c separator is ' ', a compliant \c tags string is <tt>tag1 tag2 tag3</tt>
 *
 * \note
 * the strings generated are \b cloned inside the hash table
 *
 * @param[in] section the section whose tag hash table will be populated
 * @param[in] tags a string containing "separator"-separated substring, each of them representing a tag
 * @param[in] separator a character dividing 2 tags in string \c tags
 */
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

/**
 * compute the has of a string
 *
 * \note
 * we use djb2 algorithm, described <a href="http://www.cse.yorku.ca/~oz/hash.html">here</a>
 *
 * @param[in] str the string whose has we need to compute
 * @return the has of the string
 */
int hash(const char* str) {
	unsigned int hash = 5381;
	int c;

	while ((c = *str++))
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	return hash;
}

void destroySection(Section* section) {
	if (section->firstChild != NULL) {
		destroySection(section->firstChild);
	}
	if (section->nextSibling != NULL) {
		destroySection(section->nextSibling);
	}

	Tag* current;
	Tag* tmp;
	HASH_ITER(hh, section->tags, current, tmp) {
		HASH_DEL(section->tags, current);
		destroyTag(current);
	}
	destroySectionList(&section->sectionToRunList);
	free(section->description);
	free(section);
}

void printSectionData(Section* section, bool recursive) {
	fprintf(stdout, "****************\ndescription=%s\nparent=%s\nchildrenNumber=%d\nchildrenNumberComputed=%s\nexecuted=%s\ncurrentChild=%d\nloop1=%s\n",
			section->description,
			(section->parent != NULL ? section->parent->description : "<none>"),
			section->childrenNumber,
			(section->childrenNumberComputed ? "yes" : "no"),
			(section->executed ? "yes" : "no"),
			section->currentChild,
			(section->loop1 ? "yes" : "no")
	);

	if (recursive && section->firstChild != NULL)
		printSectionData(section->firstChild, recursive);

	if (recursive && section->nextSibling != NULL) {
		printSectionData(section->nextSibling, recursive);
	}
}

bool areWeComputingChildren(Section* section) {
	return !section->childrenNumberComputed;
}

bool runOnceAndDoWorkAtEnd(Section* section, Section** pointerToSetAsParent, AfterExecutedSectionCallBack callback, AfterExecutedSectionCallBack accessGrantedCallback, AfterExecutedSectionCallBack accessDeniedCallback) {
	if (section->loop1) {
		return true;
	}
	//callback is always executed and it can (and often will) change pointerToSetAsParent and child pointers (since they point to the same structure).
	//in order to use the previously pointed structure we copy the child pointer. As for pointerToSetAsParent, we can do nothing since it will be changed nonetheless
	Section* _child = section;
	callback(pointerToSetAsParent, section);
	if (section->accessGranted) {
		accessGrantedCallback(pointerToSetAsParent, _child);
	} else {
		accessDeniedCallback(pointerToSetAsParent, _child);
	}
	return false;
}

//possible ::runOnceAndDoWorkAtEnd callbacks

void doWorkAtEndCallbackGoToParentAndThenToNextSibling(Section** pointerToSetAsParent, Section* section) {
	//we finish a section. we return to the parent
	*pointerToSetAsParent = section->parent;
	//we go to the next sibling of child
	(*pointerToSetAsParent)->currentChild += 1;
}

void doWorkAtEndCallbackChildrenNumberComputed(Section** pointerToSetAsParent, Section* section) {
	//we executed the section. Hence we can safely say we know the child number of such section
	if (!section->childrenNumberComputed) {
		section->childrenNumberComputed = true;
	}
	section->currentChild = 0;
}

void doWorkAtEndCallbackUpdateSectionToRun(Section** pointerToSetAsParent, Section* section) {
	//we executed the section. Hence we can safely say we know the child number of such section
	if (section->parent->childrenNumberComputed) {
		//since childrenNumberComputed is not false, that means that we are executing the cycle at least the second time. Hence
		//we need to pop the head of sectionToRunList. However we don't need to pop the head when we end a WHEN, but when we end a loop cycle.
		//in order to do it, we pop the end after we executed the last children
		printf("section next sibling %p\n", section->nextSibling);
		if (section->nextSibling == NULL) {
			popHeadSectionInSectionList(&(section->parent->sectionToRunList));
		}
	}
}

void doWorkAtEndCallbackUpdateSectionAndMarkChildrenComputedToRun(Section** pointerToSetAsParent, Section* section) {
	doWorkAtEndCallbackUpdateSectionToRun(pointerToSetAsParent, section);
	doWorkAtEndCallbackChildrenNumberComputed(pointerToSetAsParent, section);
}

void doWorkAtEndCallbackResetContainer(Section** pointerToSetAsParent, Section* child) {
	//we finished a section. Hence now we know the number of children that section have
	child->childrenNumberComputed = true;
	child->currentChild = 0;
}

void doWorkAtEndCallbackChildrenNumberComputedListGoToParentAndThenToNextSibling(Section** pointerToSetAsParent, Section* section) {
	//in the first cycle, when we're trying to compute the number of children of the parent, we also initialize the sectionToRunList
	if (!section->parent->childrenNumberComputed) {
		//we can add every children of parent except the first one: such child has already run while we were computing the number of children
		if (section->parent->currentChild > 0) {
			addSectionInTailSectionList(&(section->parent->sectionToRunList), section);
		}
	}

	doWorkAtEndCallbackGoToParentAndThenToNextSibling(pointerToSetAsParent, section);
}

void doWorkAtEndCallbackDoNothing(Section** pointerToSetAsParent, Section* section) {

}

//END

bool runOnceAndCheckAccessToSection(Section* section, condition_section cs, BeforeStartingSectionCallBack callback) {
	if (!section->loop2) {
		return false;
	}
	printf("checking if we can access to \"%s\"... ", section->description);
	section->accessGranted = cs(section);
	printf(section->accessGranted ? "yes" : "no");
	printf("\n");
	if (section->accessGranted) {
		callback(section);
	}
	return section->accessGranted;
}

//possible condition_section filters

/**
 * Grants always access to \c section
 */
bool getAlwaysTrue(Section* section) {
	return true;
}

/**
 * Grant access only if:
 * \li we're still computing the number of children and we are in the fuirst children;
 * \li we have already computed the number of children and the children we're analyzing is the one in the head of ::Section::sectionToRunList
 */
bool getAccessSequentially(Section* section) {
	if (section->parent == NULL) {
		return true;
	}

	if (section->parent->childrenNumberComputed == false) {
		return (section->parent->currentChild == 0);
	}

	if (peekSectionInHeadSectionList(&(section->parent->sectionToRunList)) == section) {
		return true;
	}

	return false;
}

//END

//accessGrantedCallBack callbacks

void callbackDoNothing(Section* section) {
}

//END

bool haveWeRunEverythingInSection(Section* section) {
	return section->executed;
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

void markSectionAsExecuted(Section* section) {
	section->executed = true;
	printf("%s completed\n", section->description);
	SectionCell* tmp = section->sectionToRunList;
	printf("sectionToRunList: ");
	while (tmp != NULL) {
		printf("%s ", tmp->section->description);
		tmp = tmp->next;
	}
	printf("\n");
}

Section* getSectionOrCreateIfNotExist(Section* parent, SectionLevelId sectionLevelId, const char* decription, const char* tags) {
	if (areWeComputingChildren(parent)) {
		parent->childrenNumber += 1;
		return addSectionToParent(initSection(sectionLevelId, decription, tags), parent);
	}
	return getNSection(parent, parent->currentChild);
}

#define CONTAINABLESECTION(parent, sectionLevelId, description, tags, condition, accessGrantedCallBack, getBackToParentCallBack, exitFromContainerAccessGrantedCallback, exitFromContainerAccessDeniedCallback, setupCode)	\
		/**
		 * Every time we enter inside a section (WHEN, THEN, TESTCASE) we
		 * create a new metadata data representing such section (if not created yet)
		 * and then we enter in such section. At the end of the execution,
		 * we return to the parent section
		 */																																\
		currentSection = getSectionOrCreateIfNotExist(parent, sectionLevelId, description, tags);												\
		currentSection->loopId += 1;																									\
		setupCode																														\
		for (																															\
				currentSection->loop1 = true																							\
				;																														\
				runOnceAndDoWorkAtEnd(currentSection, &currentSection, 																	\
						getBackToParentCallBack, exitFromContainerAccessGrantedCallback, exitFromContainerAccessDeniedCallback			\
				)																														\
				;																														\
				/**
				 *  This code is execute when we have already executed the code
				 *  inside the container. We assume every post condition of
				 *  CONTAINABLESECTION is satisfied for its children
				 *  CONTAINABLESECTION.
				 */																														\
		currentSection->loop1 = false																							\
		)																																\
		for (																															\
				currentSection->loop2 = true																							\
				;																														\
				runOnceAndCheckAccessToSection(currentSection, condition, accessGrantedCallBack)										\
				;																														\
				currentSection->loop2 = false,																							\
				markSectionAsExecuted(currentSection)																					\
		)

#define NOCODE

#define LOOPER(parent, sectionLevelId, description, tags)																				\
		CONTAINABLESECTION(																												\
				parent, sectionLevelId, description, tags,																				\
				getAlwaysTrue, callbackDoNothing, 																						\
				doWorkAtEndCallbackResetContainer, doWorkAtEndCallbackDoNothing, doWorkAtEndCallbackDoNothing,							\
				for (																													\
						;																												\
						!haveWeRunEveryChildrenInSection(currentSection)																\
						;																												\
						\
				)																														\
		)


#define TESTCASE(description, tags) LOOPER(&rootSection, 1, description, tags)
#define EZ_TESTCASE(description) TESTCASE(description, "")

#define ALWAYS_ENTER(sectionLevelId, description, tags) CONTAINABLESECTION(																\
		currentSection, sectionLevelId, description, tags,																				\
		getAlwaysTrue, callbackDoNothing,																								\
		doWorkAtEndCallbackGoToParentAndThenToNextSibling,	doWorkAtEndCallbackChildrenNumberComputed, doWorkAtEndCallbackDoNothing,					\
		NOCODE																															\
)

#define ENTER_ONE_PER_LOOP(sectionLevelId, description, tags) CONTAINABLESECTION(														\
		currentSection, sectionLevelId, description, tags,																				\
		getAccessSequentially, callbackDoNothing, 																						\
		doWorkAtEndCallbackChildrenNumberComputedListGoToParentAndThenToNextSibling, doWorkAtEndCallbackUpdateSectionAndMarkChildrenComputedToRun, doWorkAtEndCallbackUpdateSectionToRun,		\
		NOCODE																															\
)

#define WHEN(description, tags) ENTER_ONE_PER_LOOP(5, description, tags)
#define EZ_WHEN(description) WHEN(description, "")
#define THEN(description, tags) ALWAYS_ENTER(10, description, tags)
#define EZ_THEN(description) THEN(description, "")

#endif
