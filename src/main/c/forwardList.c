/*
 * List.c
 *
 *  Created on: Dec 22, 2016
 *      Author: koldar
 */

#include <stdlib.h>
#include "forwardList.h"
#include "errors.h"

forward_list* initForwardList() {
	forward_list* retVal = (forward_list*) NULL;
	return retVal;
}

void destroyForwardList(forward_list** list) {
	forward_list* tmp = NULL;
	forward_list* tmp2 = NULL;

	tmp = *list;
	while (tmp != NULL) {
		tmp2 = tmp->next;
		free(tmp);
		tmp = tmp2;
	}
}

void destroyForwardListWithElements(forward_list** list, destructor d) {
	forward_list* tmp = NULL;
	forward_list* tmp2 = NULL;

	tmp = *list;
	while (tmp != NULL) {
		tmp2 = tmp->next;
		d(tmp->pointer);
		free(tmp);
		tmp = tmp2;
	}
}


void addHeadInForwardList(forward_list** list, const void* pointer) {
	forward_list* newElement = (forward_list*)malloc(sizeof(forward_list));
	if (newElement == NULL) {
		MALLOCERRORCALLBACK();
	}

	newElement->pointer = pointer;
	newElement->next = *list;

	*list = newElement;
}

void* findInForwardList(forward_list** list, comparator c) {
	ForwardCell* tmp = *list;
	while (tmp != NULL) {
		if (c(tmp->pointer) == true) {
			return tmp->pointer;
		}
		tmp = tmp->next;
	}
	return NULL;
}

bool removeFindInForwardList(forward_list** list, comparator c) {
	forward_list* prevComponent = NULL;
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

bool removeFindWithElementInForwardList(forward_list** list, comparator c, destructor d) {
	forward_list* prevComponent = NULL;
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

void* popHeadFromForwardList(forward_list** list) {
	if (isForwardListEmpty(*list)) {
		return NULL;
	}
	forward_list* tmp = (*list);

	void* retVal = (*list)->pointer;
	*list = (*list)->next;
	free(tmp);
	return retVal;
}

void* peekHeadFromForwardList(const forward_list** list) {
	return isForwardListEmpty(*list) ? NULL : (*list)->pointer;
}

bool isForwardListEmpty(const forward_list** list) {
	return (*list) == NULL;
}

int getForwardListSize(const forward_list** list) {
	forward_list* tmp = *list;
	int retVal = 0;
	while (tmp != NULL) {
		retVal++;
		tmp = tmp->next;
	}
	return retVal;
}

forward_list* getTailInForwardList(const forward_list** list) {
	forward_list* tmp = *list;

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

void appendForwardListToTail(forward_list** dest, const forward_list** src) {
	forward_list* destTail = getTailInForwardList(*dest);
	forward_list* tmp = src;

	destTail->next = src;
}

void clearForwardList(forward_list** toClear) {
	forward_list* tmp = *toClear;
	forward_list* tmp2 = NULL;
	while (tmp != NULL) {
		tmp2 = tmp->next;
		free(tmp);
		tmp = tmp2;
	}
	*toClear = NULL;
}

void clearForwardListWithElements(forward_list** toClear, destructor d) {
	forward_list* tmp = *toClear;
	forward_list* tmp2 = NULL;
	while (tmp != NULL) {
		tmp2 = tmp->next;
		d(tmp->pointer);
		free(tmp);
		tmp = tmp2;
	}
	*toClear = NULL;
}
