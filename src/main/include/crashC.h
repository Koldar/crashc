#ifndef CRASHC_H_
#define CRASHC_H_

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct Section {
	const char* description;

	bool childrenNumberComputed;
	int childrenNumber;
	bool executed;

	int currentChild;

	bool loop1;
	bool loop2;

	struct Section* parent;
	struct Section* firstChild;
	struct Section* nextSibling;
} Section;

typedef bool (*condition_section)(Section*);
typedef void (*ContainableSectionTerminateCallBack)(Section* parent, Section* child);

Section rootSection = {"root", false, 0, 0, false, NULL, NULL, NULL};
Section* currentSection = NULL;

#define MALLOCERRORCALLBACK()

Section* addSectionToParent(Section* toAdd, Section* parent);
Section* getNSection(Section* parent, int nChild);
Section* initSection(const char* description);
void destroySection(Section* section);
void printSectionData(Section* section, bool recursive);
bool areWeComputingChildren(Section* section);
bool runOnceAndDoWork(Section* child, Section** newCurrentSection, ContainableSectionTerminateCallBack callback);
bool haveWeRunEverythingInSection(Section* section);
bool haveWeRunEveryChildrenInSection(Section* section);
void markSectionAsExecuted(Section* section);
Section* getSectionOrCreateIfNotExist(Section* parent, const char* decription);

bool getAlwaysTrue(Section* section);

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

Section* initSection(const char* description) {
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

bool runOnceAndDoWork(Section* child, Section** newCurrentSection, ContainableSectionTerminateCallBack callback) {
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

bool runOnceAndCheckAccessToSection(Section* section, condition_section cs) {
	if (!section->loop2) {
		return false;
	}
	return cs(section);
}

bool getAlwaysTrue(Section* section) {
	return true;
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

Section* getSectionOrCreateIfNotExist(Section* parent, const char* decription) {
	if (areWeComputingChildren(parent)) {
		parent->childrenNumber += 1;
		return addSectionToParent(initSection(decription), parent);
	}
	return getNSection((parent), (parent)->currentChild);
}

#define CONTAINABLESECTION(parent, description, condition, callGetSection, setupCode, getBackToParentCallBack)		\
		/**
		 * Every time we enter inside a section (WHEN, THEN, TESTCASE) we
		 * create a new metadata data representing such section (if not created yet)
		 * and then we enter in such section. At the end of the execution,
		 * we return to the parent section
		 */																											\
		 if (callGetSection) {																						\
			 currentSection = getSectionOrCreateIfNotExist(parent, description);									\
		 }																											\
		 setupCode																									\
		 for (																										\
				 currentSection->loop1 = true																		\
				 ;																									\
				 runOnceAndDoWork(currentSection, &currentSection, getBackToParentCallBack)							\
				 ;																									\
				 /**
				  *  This code is execute when we have already executed the code
				  *  inside the container. We assume every post condition of
				  *  CONTAINABLESECTION is satisfied for its children
				  *  CONTAINABLESECTION.
				  */																								\
				  currentSection->loop1 = false																		\
		 )																											\
		 for (																										\
				 currentSection->loop2 = true																		\
				 ;																									\
				 runOnceAndCheckAccessToSection(currentSection, condition)											\
				 ;																									\
				 currentSection->loop2 = false,																		\
				 markSectionAsExecuted(currentSection)																\
		 )

#define NOCODE ;

#define LOOPER(parent, description)																					\
		CONTAINABLESECTION(parent, description, getAlwaysTrue, true, 												\
				for (																								\
						;																							\
						!haveWeRunEveryChildrenInSection(currentSection)											\
						;																							\
						currentSection->currentChild = 0															\
				),																									\
				callbackResetContainer																				\
		)


#define TESTCASE(description) LOOPER(&rootSection, description)

#define WHEN(description) CONTAINABLESECTION(currentSection, description, getAlwaysTrue, true, NOCODE, callbackGoToParentAndThenToNextSibling)
#define THEN(description) CONTAINABLESECTION(currentSection, description, getAlwaysTrue, true, NOCODE, callbackGoToParentAndThenToNextSibling)

#endif
