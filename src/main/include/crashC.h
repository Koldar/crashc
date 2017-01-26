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

	int currentChild;

	bool loop1;
	bool loop2;

	struct Section* parent;

	struct Section* firstChild;
	struct Section* nextSibling;
} Section;

typedef bool (*condition_section)(Section*);

Section rootSection = {"root", false, 0, 0, false, NULL, NULL, NULL};
Section* currentSection = NULL;

#define MALLOCERRORCALLBACK()

Section* addSectionToParent(Section* toAdd, Section* parent);
Section* getNSection(Section* parent, int nChild);
bool runOnceAndThenGoToParent(Section* child, Section** newCurrentSection);
Section* initSection(const char* description);
void destroySection(Section* section);
void printSectionData(Section* section);
bool areWeComputingChildren(Section* section);
bool runOnceAndCheckAccessToSection(Section* section, condition_section cs);

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

void printSectionData(Section* section) {
	fprintf(stdout, "****************\ndescription=%s\nparent=%s\nchildrenNumber=%d\nchildrenNumberComputed=%s\ncurrentChild=%d\nloop1=%s\n",
			section->description,
			(section->parent != NULL ? section->parent->description : "<none>"),
			section->childrenNumber,
			(section->childrenNumberComputed ? "yes" : "no"),
			section->currentChild,
			(section->loop1 ? "yes" : "no")
	);

	if (section->firstChild != NULL)
		printSectionData(section->firstChild);

	if (section->nextSibling != NULL) {
		printSectionData(section->nextSibling);
	}
}

bool areWeComputingChildren(Section* section) {
	return !section->childrenNumberComputed;
}

bool runOnceAndThenGoToParent(Section* child, Section** newCurrentSection) {
	if (child->loop1) {
		return true;
	}
	//we finished a section. Hence now we know the number of children that section have
	child->childrenNumberComputed = true;
	//we finish a section. we return to the parent
	*newCurrentSection = child->parent;
	//we just finished analyzing a child of the parent. We need to increase the child we're analyzing
	(*newCurrentSection)->currentChild += 1;
	return false;
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

#define LOOPER(parent, description)														\
		for (																			\
		;																				\
		;																				\
		)																				\

#define CONTAINABLESECTION(parent, description, condition)								\
		/**
		 * Every time we enter inside a section (WHEN, THEN, TESTCASE) we
		 * create a new metadata data representing such section (if not created yet)
		 * and then we enter in such section. At the end of the execution,
		 * we return to the parent section
		 */																				\
		if (areWeComputingChildren(parent)) {											\
			(parent)->childrenNumber += 1;												\
			currentSection = addSectionToParent(initSection(description), (parent));	\
		} else {																		\
			currentSection = getNSection((parent), (parent)->currentChild);				\
		}																				\
		for (																			\
				currentSection->loop1 = true											\
				;																		\
				runOnceAndThenGoToParent(currentSection, &currentSection)				\
				;																		\
				/**
				 *  This code is execute when we have already executed the code
				 *  inside the container. We assume every post condition of
				 *  CONTAINABLESECTION is satisfied for its children
				 *  CONTAINABLESECTION.
				 */																		\
				currentSection->loop1 = false											\
		)																				\
			for (																		\
					currentSection->loop2 = true										\
					;																	\
					runOnceAndCheckAccessToSection(currentSection, condition)			\
					;																	\
					currentSection->loop2 = false										\
			)

#define TESTCASE(description)															\
		currentSection = &rootSection;													\
		CONTAINABLESECTION(currentSection, description, getAlwaysTrue)
#define WHEN(description) CONTAINABLESECTION(currentSection, description, getAlwaysTrue)
#define THEN(description) CONTAINABLESECTION(currentSection, description, getAlwaysTrue)

#endif
