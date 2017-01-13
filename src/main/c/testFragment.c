/*
 * testFragment.c
 *
 *  Created on: Jan 4, 2017
 *      Author: koldar
 */

#include "testFragment.h"
#include "utility.h"
#include <stdlib.h>

TestNode* initSourceCodeTestNode(TestNodeId id, fragment_function* function) {
	TestNode* retVal = (TestNode*)malloc(sizeof(TestNode));
	if (retVal == NULL) {
		errorMalloc();
	}

	retVal->id = id;
	retVal->type = FT_LEAF;
	retVal->code.function = function;
	retVal->next = NULL;

	return retVal;
}

TestNode* initIntermediateTestNode(TestCaseId id, const char* description, const char* tags) {
	TestNode* retVal = (TestNode*)malloc(sizeof(TestNode));
	if (retVal == NULL) {
		errorMalloc();
	}


	retVal->id = id;
	retVal->type = FT_INTERMEDIATE;
	retVal->container.firstChild = NULL;
	retVal->next = NULL;
	retVal->container.description = (char*) malloc(strlen(description) + 1);
	strcpy(retVal->container.description, description);
	retVal->container.tags = (char*) malloc(strlen(description) + 1);
	strcpy(retVal->container.tags, tags);

	return retVal;
}

void destroyTestFragment(TestNode* testNode) {
	switch (testNode->type) {
	case FT_LEAF: {
		break;
	}
	case FT_INTERMEDIATE: {
		TestFragmentNodeList list = testNode->container->firstChild;
		TestNode* tmp = NULL;
		while (list != NULL) {
			tmp = list->next;
			destroyTestFragment(list);
			list = tmp;
		}
		testNode->container.
	}
	}
	free(testNode);
}


