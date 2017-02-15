/*
 * List.c
 *
 *  Created on: Dec 22, 2016
 *      Author: koldar
 */

#include <stdlib.h>
#include "forwardList.h"

ForwardList* initForwardList() {
	ForwardList* retVal = (ForwardList*) NULL;
	return retVal;
}

void destroyForwardList(ForwardList** list) {
	ForwardList* tmp = NULL;
	ForwardList* tmp2 = NULL;

	tmp = *list;
	while (tmp != NULL) {
		tmp2 = tmp->next;
		free(tmp);
		tmp = tmp2;
	}
}

void destroyForwardListWithElements(ForwardList** list, destructor d) {
	ForwardList* tmp = NULL;
	ForwardList* tmp2 = NULL;

	tmp = *list;
	while (tmp != NULL) {
		tmp2 = tmp->next;
		d(tmp->pointer);
		free(tmp);
		tmp = tmp2;
	}
}


void addHead(ForwardList** list, const void* pointer) {
	ForwardList* newElement = (ForwardList*)malloc(sizeof(ForwardList));
	if (newElement == NULL) {
		errorMalloc();
	}

	newElement->pointer = pointer;
	newElement->next = *list;

	*list = newElement;
}

bool isListEmpty(const ForwardList** list) {
	return (*list) == NULL;
}

ForwardList* getTail(const ForwardList** list) {
	ForwardList* tmp = *list;

	if (tmp == NULL) {
		return NULL;
	}
	while (true) {
		if (tmp->next == NULL) {
			return tmp;
		}
		tmp = tmp->next;
	}
}

void appendListToTail(ForwardList** dest, const ForwardList** src) {
	ForwardList* destTail = getLastElement(*dest);
	ForwardList* tmp = src;
	while (tmp != NULL) {
		if (destTail == NULL) {
			addHeadInForwardList(dest, tmp->pointer);
			destTail = *dest;
		} else {
			addElementAfterForwardList(destTail, tmp->pointer);
		}
		tmp = tmp->next;
	}
}

void clearForwardList(ForwardList** toClear) {
	ForwardList* tmp = *toClear;
	ForwardList* tmp2 = NULL;
	while (tmp != NULL) {
		tmp2 = tmp->next;
		free(tmp);
		tmp = tmp2;
	}
	*toClear = NULL;
}

void clearForwardListWithElements(ForwardList** toClear, destructor d) {
	ForwardList* tmp = *toClear;
	ForwardList* tmp2 = NULL;
	while (tmp != NULL) {
		tmp2 = tmp->next;
		d(tmp->pointer);
		free(tmp);
		tmp = tmp2;
	}
	*toClear = NULL;
}

void* find(ForwardList** list, comparator c) {
	ForwardCell* tmp = *list;
	while (tmp != NULL) {
		if (c(tmp->pointer) == true) {
			return tmp->pointer;
		}
		tmp = tmp->next;
	}
	return NULL;
}

/**
 * like ::find but it removes the element from the list once found
 *
 * \todo create a double pointer
 *
 * \note if the element is not found, the function does nothing
 *
 * @param[in] list the list involved
 * @param[in] c the comparator used to search the list
 */
bool removeFind(ForwardList** list, comparator c) {
	ForwardList* prevComponent = NULL;
	ForwardCell* tmp = *list;

	while (tmp != NULL) {
		if (c(tmp->pointer) == true) {
			if (prevComponent == NULL) {
				//we're removing the head
				*list = tmp->next;
			} else {
				prevComponent->next = tmp->next;
			}
			free(tmp);
			return true;
		}
		prevComponent = tmp;
		tmp = tmp->next;
	}
	return false;
}

bool removeFindWithElement(ForwardList** list, comparator c, destructor d) {
	ForwardList* prevComponent = NULL;
	ForwardCell* tmp = *list;

	while (tmp != NULL) {
		if (c(tmp->pointer) == true) {
			if (prevComponent == NULL) {
				//we're removing the head
				*list = tmp->next;
			} else {
				prevComponent->next = tmp->next;
			}
			d(tmp->pointer);
			free(tmp);
			return true;
		}
		prevComponent = tmp;
		tmp = tmp->next;
	}
	return false;
}

bool isForwardListEmpty(const ForwardList** list) {
	return (*list) == NULL;
}

int getForwardListSize(const ForwardList** list) {
	ForwardList* tmp = *list;
	int retVal = 0;
	while (tmp != NULL) {
		retVal++;
		tmp = tmp->next;
	}
	return retVal;
}

void* popHeadFromList(ForwardList** list) {
	if (isForwardListEmpty(*list)) {
		return NULL;
	}
	ForwardList* tmp = (*list);

	void* retVal = (*list)->pointer;
	*list = (*list)->next;
	free(tmp);
	return retVal;
}

void* peekHeadFromList(const ForwardList** list) {
	return isForwardListEmpty(*list) ? NULL : (*list)->pointer;
}
