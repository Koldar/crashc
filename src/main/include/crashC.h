#ifndef CRASHC_H_
#define CRASHC_H_

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef int SectionLevelId;

typedef struct SectionLevelIdList {
	SectionLevelId levelId;
	struct SectionLevelIdList* next;
} SectionLevelIdList;

typedef struct Section {
	SectionLevelId levelId;
	const char* description;

	bool childrenNumberComputed;
	int childrenNumber;
	bool executed;

	int currentChild;

	bool loop1;
	bool loop2;

	SectionLevelIdList* levelIdBlackList;

	struct Section* parent;
	struct Section* firstChild;
	struct Section* nextSibling;
} Section;

typedef bool (*condition_section)(Section*);
typedef void (*SectionTerminatedCallBack)(Section* parent, Section* child);
typedef void (*AccessGrantedCallBack)(Section* sectionGranted);

Section rootSection = {0, "root", false, 0, 0, false, NULL, NULL, NULL};
Section* currentSection = NULL;

#define MALLOCERRORCALLBACK()

SectionLevelIdList* initSectionLevelIdList();
void addSectionLevelIdList(SectionLevelIdList** list, SectionLevelId id);
bool containsSectionLevelIdList(SectionLevelIdList** list, SectionLevelId id);
bool removeSectionLevelIdList(SectionLevelIdList** list, SectionLevelId id);
void destroySectionLevelIdList(SectionLevelIdList** list);
Section* addSectionToParent(Section* toAdd, Section* parent);
Section* getNSection(Section* parent, int nChild);
Section* initSection(SectionLevelId levelId, const char* description);
void destroySection(Section* section);
void printSectionData(Section* section, bool recursive);
bool areWeComputingChildren(Section* section);
bool runOnceAndCheckAccessToSection(Section* section, condition_section cs, AccessGrantedCallBack callback);
bool runOnceAndDoWorkAtEnd(Section* child, Section** newCurrentSection, SectionTerminatedCallBack callback);
bool haveWeRunEverythingInSection(Section* section);
bool haveWeRunEveryChildrenInSection(Section* section);
void markSectionAsExecuted(Section* section);
Section* getSectionOrCreateIfNotExist(Section* parent, SectionLevelId sectionLevelId, const char* decription);

bool getAlwaysTrue(Section* section);

SectionLevelIdList* initSectionLevelIdList() {
	return NULL;
}

void addSectionLevelIdList(SectionLevelIdList** list, SectionLevelId id) {
	SectionLevelIdList* retVal = malloc(sizeof(SectionLevelIdList));
	if (retVal == NULL) {
		MALLOCERRORCALLBACK();
	}

	retVal->levelId = id;
	retVal->next = NULL;

	if ((*list) == NULL) {
		*list = retVal;
	} else {
		retVal->next = *list;
		*list = retVal;
	}
}

bool removeSectionLevelIdList(SectionLevelIdList** list, SectionLevelId id) {
	SectionLevelIdList* tmp = *list;
	SectionLevelIdList* previous = NULL;
	while (tmp != NULL) {
		if (tmp->levelId == id) {
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

bool containsSectionLevelIdList(SectionLevelIdList** list, SectionLevelId id) {
	SectionLevelIdList* tmp = *list;
	while (tmp != NULL) {
		if (tmp->levelId == id) {
			return true;
		}
		tmp = tmp->next;
	}
	return false;
}

void destroySectionLevelIdList(SectionLevelIdList** list) {
	SectionLevelIdList* tmp = *list;
	SectionLevelIdList* tmp2 = NULL;
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

Section* initSection(SectionLevelId levelId, const char* description) {
	Section* retVal = malloc(sizeof(Section));
	if (retVal == NULL) {
		MALLOCERRORCALLBACK();
	}

	retVal->childrenNumber = 0;
	retVal->childrenNumberComputed = false;
	retVal->executed = false;
	retVal->currentChild = 0;
	retVal->description = strdup(description);
	retVal->firstChild = NULL;
	retVal->levelId = levelId;
	retVal->levelIdBlackList = initSectionLevelIdList();
	retVal->loop1 = false;
	retVal->nextSibling = NULL;
	retVal->parent = NULL;

	return retVal;
}
void destroySection(Section* section) {
	if (section->firstChild != NULL) {
		destroySection(section->firstChild);
	}
	if (section->nextSibling != NULL) {
		destroySection(section->nextSibling);
	}

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

bool runOnceAndDoWorkAtEnd(Section* child, Section** newCurrentSection, SectionTerminatedCallBack callback) {
	if (child->loop1) {
		return true;
	}
	callback(newCurrentSection, child);
	return false;
}

void callbackGoToParentAndThenToNextSibling(Section** parentPointer, Section* child) {
	//we finished a section. Hence now we know the number of children that section have
	child->childrenNumberComputed = true;
	//we finish a section. we return to the parent
	*parentPointer = child->parent;
	//we go to the next sibling of child
	(*parentPointer)->currentChild += 1;
}

void callbackResetContainer(Section** parentPointer, Section* child) {
	//we finished a section. Hence now we know the number of children that section have
	child->childrenNumberComputed = true;
}

bool runOnceAndCheckAccessToSection(Section* section, condition_section cs, AccessGrantedCallBack callback) {
	if (!section->loop2) {
		return false;
	}
	bool retVal = cs(section);
	if (retVal) {
		callback(section);
	}
	return retVal;
}

void callbackDoNothing(Section* section) {

}

bool getAlwaysTrue(Section* section) {
	return true;
}

bool getAccessOnlyIfNotInBlackList(Section* section) {
	if (section->parent == NULL) {
		return true;
	}
	return containsSectionLevelIdList(&section->parent->levelIdBlackList, section->levelId) ? false : true;
}

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
	printf("%s completed\n", currentSection->description);
}

Section* getSectionOrCreateIfNotExist(Section* parent, SectionLevelId sectionLevelId, const char* decription) {
	if (areWeComputingChildren(parent)) {
		parent->childrenNumber += 1;
		return addSectionToParent(initSection(sectionLevelId, decription), parent);
	}
	return getNSection((parent), (parent)->currentChild);
}

#define CONTAINABLESECTION(parent, sectionLevelId, description, condition, getBackToParentCallBack, accessGrantedCallBack, setupCode)		\
		/**
		 * Every time we enter inside a section (WHEN, THEN, TESTCASE) we
		 * create a new metadata data representing such section (if not created yet)
		 * and then we enter in such section. At the end of the execution,
		 * we return to the parent section
		 */																																\
		 currentSection = getSectionOrCreateIfNotExist(parent, sectionLevelId, description);											\
		 setupCode																														\
		 for (																															\
				 currentSection->loop1 = true																							\
				 ;																														\
				 runOnceAndDoWorkAtEnd(currentSection, &currentSection, getBackToParentCallBack)										\
				 ;																														\
				 /**
				  *  This code is execute when we have already executed the code
				  *  inside the container. We assume every post condition of
				  *  CONTAINABLESECTION is satisfied for its children
				  *  CONTAINABLESECTION.
				  */																													\
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

#define NOCODE ;

#define LOOPER(parent, sectionLevelId, description)																						\
		CONTAINABLESECTION(parent, sectionLevelId, description, getAlwaysTrue, callbackResetContainer, callbackDoNothing,				\
				for (																													\
						;																												\
						!haveWeRunEveryChildrenInSection(currentSection)																\
						;																												\
				)																														\
		)


#define TESTCASE(description) LOOPER(&rootSection, 1, description)

#define ALWAYS_ENTER(sectionLevelId, description) CONTAINABLESECTION(currentSection, sectionLevelId, description, getAlwaysTrue, callbackGoToParentAndThenToNextSibling, callbackDoNothing, NOCODE)
//#define ENTER_ONE_PER_LOOP(sectionLevelId, description) CONTAINABLEECTION(currentSection, sectionLevelId, description, )



#define WHEN(description) CONTAINABLESECTION(currentSection, 5, description, getAlwaysTrue, callbackGoToParentAndThenToNextSibling, callbackDoNothing, NOCODE)
#define THEN(description) CONTAINABLESECTION(currentSection, 10, description, getAlwaysTrue, callbackGoToParentAndThenToNextSibling, callbackDoNothing, NOCODE)

#endif
