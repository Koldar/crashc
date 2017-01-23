/*
 * crashC.h
 *
 *  Created on: Jan 4, 2017
 *      Author: koldar
 */

#ifndef CRASHC_H_
#define CRASHC_H_




#define STR(x) #x
#define ARGSTR(x) STR(x)

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_SECTION_DEPTH 10

typedef struct Run {
	const char* id;

	bool hasComputedChildrenRunNumber;
	bool completed;

	int currentChild;
	int nextChildToRun;
	int totalChildrenRunNumber;

	bool accessLoop;
	bool accessLoop2;

	struct Run* firstChild;
	struct Run* nextSibling;
	struct Run* parent;
} Run;
typedef Run RootRun;

static RootRun* rootRun;
static Run* currentRun;

Run* initRun(const char* id);
void destroyRun(Run* run);
void clearRuns(RootRun* rootRun);
void addRunOnTail(Run* list, Run* run);
Run* getNRun(RootRun* rootRun, int n);
Run* getNextRun(Run* run);

#define LOOPCONTAINER(id) \
		clearRuns(rootRun); \
		rootRun = (RootRun*) initRun(id); \
		currentRun = rootRun;\
		\
		for( \
				; \
				!currentRun->completed; \
				\
		)

#define TESTCONTAINER(id) \
		/*when we run this currentRun refers to the test container parent!*/ \
		if (!currentRun->hasComputedChildrenRunNumber) { \
			/*we still need to know how many child there are in this container. We got a child container hence we increase the number by 1*/ \
			currentRun->totalChildrenRunNumber++; \
			addToRunChildren(currentRun, initRun(id)); \
		} \
		/**
		 * This first is only here to increment currentChild at the end of the child container. The function does the
		 * increment netherless: either if we enter in the test container or not. Since we cannot define a flag (since test containers
		 * can be nested) we use a flag inside currentRun. When the increase statement of the loop is execute, we have scanned all the children
		 * inside the container. Hence we can set the hasComputedChildrenRunNumber flag to true
		 */ \
		for ( \
				currentRun->accessLoop = false; \
				\
				!currentRun->accessLoop; \
				\
				/**
				 * If the test container contains only one other test container, the first (and the last) loop where we execute its children
				 * won't set the complete flag. Hence the child won't update the parent next children to execute. completeChildIfUnique
				 * deals with that
				 */ \
				currentRun->currentChild++, \
				currentRun->hasComputedChildrenRunNumber = true, \
				completeChildIfUnique(currentRun) \
			) \
			/** This second for checks if we need to enter in the container:
			 * if this is the first child and we're still getting the number of children inside the container that we
			 * should execute the child (in the first run we still need to execute something! Otheriwse we execute the
			 * container only if there it's the child turn to be executed). The third line of the loop is executed only
			 * if we actually enter inside the test container: we need it to return to the parent since after this container we may encouter
			 * a different one.
			 */ \
			for ( \
				currentRun->accessLoop = true; \
				(currentRun->currentChild == currentRun->nextChildToRun) && (((!currentRun->hasComputedChildrenRunNumber)&&(currentRun->currentChild == 0))||(currentRun->hasComputedChildrenRunNumber)); \
				\
			) \
				/*NOW WE ARE INSIDE THE TEST CONTAINER SINCE CURRENTRUN POINTS TO THE TEST CONTAINER
				 * This is a dummy if: it it always true but we need it to properly set currentRun*/ \
				for \
				( \
						currentRun->accessLoop2 = true; \
						currentRun = getChild(currentRun, currentRun->nextChildToRun), \
						\
						currentRun->accessLoop2; \
						\
						currentRun->nextChildToRun += (currentRun->completed ? 0 : 1), \
						currentRun->completed = currentRun->hasComputedChildrenRunNumber ? currentRun->nextChildToRun == currentRun->totalChildrenRunNumber : false, \
						goToNextSibling(currentRun),\
						currentRun = currentRun->parent, \
						currentRun->accessLoop2 = false \
				) \

#define TESTCASE(id)	LOOPCONTAINER(id)
#define WHEN(id)		TESTCONTAINER(id)
#define THEN(id)		TESTCONTAINER(id)

void goToNextSibling(Run* run) {
	if (run->completed) {
		run->parent->nextChildToRun++;
	}
}

void completeChildIfUnique(Run* run) {
	if (run->firstChild == NULL) {
		return;
	}
	if (run->firstChild->nextSibling != NULL) {
		return;
	}
	run->firstChild->completed = true;
	run->firstChild->hasComputedChildrenRunNumber = true;

	run->nextChildToRun++;
	run->completed = true;

}

#endif /* CRASHC_H_ */
