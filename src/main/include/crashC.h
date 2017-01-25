///*
// * crashC.h
// *
// *  Created on: Jan 4, 2017
// *      Author: koldar
// */
//
//#ifndef CRASHC_H_
//#define CRASHC_H_
//
//
//
//
//#define STR(x) #x
//#define ARGSTR(x) STR(x)
//
//#include <stdlib.h>
//#include <stdio.h>
//#include <string.h>
//#include <stdbool.h>
//
//#define MAX_SECTION_DEPTH 10
//
//typedef struct Run {
//	const char* id;
//	const char* description;
//
//	bool hasComputedChildrenRunNumber;
//	bool completed;
//
//	int currentChild;
//	int nextChildToRun;
//	int totalChildrenRunNumber;
//
//	bool accessLoop1;
//	bool accessLoop2;
//	bool accessLoop3;
//
//	struct Run* firstChild;
//	struct Run* nextSibling;
//	struct Run* parent;
//} Run;
//typedef Run RootRun;
//
//static RootRun* rootRun;
//static Run* currentRun;
//
//Run* initRun(const char* id, const char* description);
//void destroyRun(Run* run);
//Run* getChild(Run* run, int n);
//void goToNextSibling(Run* run);
//void completeChildIfUnique(Run* run);
//void sprintRun(char* buffer, const Run* r);
//
//#define LOOPCONTAINER(id, name, description) \
//		destroyRun(rootRun); \
//		rootRun = (RootRun*) initRun(id, description); \
//		currentRun = rootRun;\
//		\
//		for( \
//				; \
//				!currentRun->completed; \
//				\
//		)
//
///**
// * currentRun is the run of the TESTCONTAINER
// * parentRun is the run of the container owning TESTCONTAINER
// */
//#define TESTCONTAINER(parentRun, id, name, description) \
//		/*when we run this currentRun refers to the test container parent!*/ \
//		if (!parentRun->hasComputedChildrenRunNumber) { \
//			/*we still need to know how many child there are in this container. We got a child container hence we increase the number by 1*/ \
//			parentRun->totalChildrenRunNumber++; \
//			addToRunChildren(parentRun, initRun(id, description)); \
//		} \
//		/**
//		 * This first loop execute code that needs to be run netherless: whether or not we enter in the test container
//		 * we need to execute the code
//		 */ \
//		for ( \
//			parentRun->accessLoop1 = true \
//			; \
//			parentRun->accessLoop1 \
//			; \
//			parentRun->currentChild += 1, \
//			parentRun->accessLoop1 = false \
//		) \
//			/**
//			 * The second loop control whether or not we should access in the given test container
//			 */ \
//			 for ( \
//				parentRun->accessLoop2 = true, \
//				sprintRun(stdout, currentRun) \
//				; \
//				parentRun->accessLoop2 && \
//				(!parentRun->completed) && \
//				(parentRun->currentChild == parentRun->nextChildToRun) && \
//				(((!parentRun->hasComputedChildrenRunNumber)&&(parentRun->currentChild == 0))||(parentRun->hasComputedChildrenRunNumber)) \
//				; \
//				/*
//				 * Note that this piece of code is executed only if we enter in the test container. It is the glue code connectint loop2 and loop3.
//				 * This piece of code is execute just right after ALPHA
//				 */ \
//				parentRun = currentRun->parent, \
//				parentRun->accessLoop2 = false \
//			) \
//				/**
//				 * The third loop execute code when the software grants us access to the particular test container
//				 */ \
//				for ( \
//						currentRun = getChild(parentRun, parentRun->nextChildToRun), \
//						currentRun->accessLoop3 = true, \
//						sprintRun(stdout, currentRun) \
//						; \
//						currentRun->accessLoop3 \
//						; \
//						/*
//						 * ALPHA: We have just terminated the given test container.
//						 * Hence we assume we have scanned all its children
//						 */ \
//						currentRun->hasComputedChildrenRunNumber = true, \
//						currentRun->completed = currentRun->hasComputedChildrenRunNumber ? currentRun->nextChildToRun == currentRun->totalChildrenRunNumber : false, \
//						currentRun->nextChildToRun += (currentRun->completed ? 0 : 1), \
//						currentRun->parent->nextChildToRun += (currentRun->completed ? 1 : 0), \
//						currentRun->accessLoop3 = false \
//				) \
//
//#define TESTCASE(id, description)	LOOPCONTAINER(id, "test name", description)
//#define WHEN(id, description)		TESTCONTAINER(currentRun, id, "when", description)
//#define THEN(id, description)		TESTCONTAINER(currentRun, id, "then", description)
//
//#endif /* CRASHC_H_ */


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

	struct Section* parent;

	struct Section* firstChild;
	struct Section* nextSibling;
} Section;

Section rootSection = {false, 0, 0, false, NULL, NULL, NULL};
Section* currentSection = NULL;

#define MALLOCERRORCALLBACK()

Section* addSectionToParent(Section* toAdd, Section* parent);
Section* getNSection(Section* parent, int nChild);
bool runOnceAndThenGoToParent(Section* child, Section** newCurrentSection);
Section* initSection(const char* description);
void destroySection(Section* section);
void printSectionData(Section* section);

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

bool runOnceAndThenGoToParent(Section* child, Section** newCurrentSection) {
	if (child->loop1) {
		return true;
	}
	*newCurrentSection = child->parent;
	return false;
}


#define CONTAINABLESECTION(parent, description)											\
		if (!(parent)->childrenNumberComputed) {										\
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
				currentSection->loop1 = false,											\
				currentSection->childrenNumberComputed = true,							\
				currentSection->currentChild += 1										\
		)

#define TESTCASE(description)															\
		currentSection = &rootSection;													\
		CONTAINABLESECTION(currentSection, description)
#define WHEN(description) CONTAINABLESECTION(currentSection, description)
#define THEN(description) CONTAINABLESECTION(currentSection, description)

#endif
