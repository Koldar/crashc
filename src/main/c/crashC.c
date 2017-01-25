///*
// * crashC.c
// *
// *  Created on: Jan 23, 2017
// *      Author: koldar
// */
//
//#include "crashC.h"
//
//#include <stdbool.h>
//
//Run* initRun(const char* id, const char* description) {
//	Run* retVal = malloc(sizeof(Run));
//	if (retVal == NULL) {
//		return NULL;
//	}
//
//	retVal->accessLoop1 = false;
//	retVal->accessLoop2 = false;
//	retVal->completed = false;
//	retVal->currentChild = 0;
//	retVal->description = strdup(description);
//	retVal->firstChild = NULL;
//	retVal->hasComputedChildrenRunNumber = false;
//	retVal->id = strdup(id);
//	retVal->nextChildToRun = 0;
//	retVal->nextSibling = NULL;
//	retVal->parent = NULL;
//	retVal->totalChildrenRunNumber = 0;
//
//	return retVal;
//}
//
//void destroyRun(Run* run) {
//	if (run == NULL) {
//		return;
//	}
//	if (run->nextSibling != NULL) {
//		destroyRun(run->nextSibling);
//	}
//	if (run->firstChild != NULL) {
//		destroyRun(run->firstChild);
//	}
//	free(run->id);
//	free(run->description);
//}
//
//void addToRunChildren(Run* parent, Run* child) {
//	Run* r = NULL;
//	Run* list = NULL;
//
//	child->parent = parent;
//
//	list = parent->firstChild;
//	if (list == NULL) {
//		parent->firstChild = child;
//		return;
//	}
//	while (true) {
//		if (list->nextSibling != NULL) {
//			list = list->nextSibling;
//		} else {
//			list->nextSibling = child;
//			break;
//		}
//	}
//}
//
//Run* getChild(Run* run, int n) {
//	Run* list = run->firstChild;
//	while(true) {
//		if ((n == 0) || (list == NULL)) {
//			return list;
//		} else {
//			list = list->nextSibling;
//			n--;
//		}
//	}
//}
//
//void goToNextSibling(Run* run) {
//	if (run->completed) {
//		run->parent->nextChildToRun++;
//	}
//}
//
//void completeChildIfUnique(Run* run) {
//	if (run->firstChild == NULL) {
//		return;
//	}
//	if (run->firstChild->nextSibling != NULL) {
//		return;
//	}
//	//there is only a single child
//	printf("waesdfgh\n");
//	sprintRun(stdout, run);
//	run->firstChild->completed = true;
//	run->firstChild->hasComputedChildrenRunNumber = true;
//
//	run->nextChildToRun++;
//	run->completed = true;
//
//}
//
//void sprintRun(char* buffer, const Run* r) {
//	fprintf(stdout, "**********************\nid=\"%s\"\ndescription=\"%s\"\nhasComputedChildrenRunNumber=%s\ncompleted=%s\ntotalChildrenNumber=%d\ncurrentChild=%d\nnextChildToRun=%d\naccessLoop1=%s\naccessLoop3=%s\naccessLoop3=%s\n",
//			r->id,
//			r->description,
//			(r->hasComputedChildrenRunNumber ? "yes" : "no"),
//			(r->completed ? "yes" : "no"),
//			r->totalChildrenRunNumber,
//			r->currentChild,
//			r->nextChildToRun,
//			(r->accessLoop1 ? "yes" : "no"),
//			(r->accessLoop2 ? "yes" : "no"),
//			(r->accessLoop3 ? "yes" : "no")
//	);
//}
