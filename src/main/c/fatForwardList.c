///*
// * List.c
// *
// *  Created on: Dec 22, 2016
// *      Author: koldar
// */
//
//#include <stdlib.h>
//#include "fatForwardList.h"
//#include "errors.h"
//
//static forward_cell* getTailCellInForwardList(const forward_list** list);
//
//forward_list* initFatForwardList() {
//	fat_forward_list* retVal = malloc(sizeof(fat_forward_list));
//
//	retVal->list = initForwardList();
//	retVal->size = 0;
//	retVal->tail = NULL;
//
//	return retVal;
//}
//
//void destroyFatForwardList(fat_forward_list** list) {
//	destroyForwardList(&((*list)->list));
//	free(*list);
//}
//
//void destroyFatForwardListWithElements(fat_forward_list** list, destructor d) {
//	destroyForwardListWithElements(&((*list)->list), d);
//	free(*list);
//}
//
//
//void addHeadInFatForwardList(fat_forward_list** list, const void* pointer) {
//	addHeadInForwardList(&((*list)->list), pointer);
//	(*list)->size++;
//	if ((*list)->tail == NULL) {
//		(*list)->tail = (*list)->list;
//	}
//}
//
//void addTailInFatForwardList(fat_forward_list** list, const void* pointer) {
//	forward_list* newElement = malloc(sizeof(forward_list));
//	if (newElement == NULL) {
//		MALLOCERRORCALLBACK();
//	}
//
//	newElement->pointer = pointer;
//	newElement->next = NULL;
//
//	if ((*list)->tail == NULL) {
//		addHeadInForwardList(&(*list)->list, pointer);
//		(*list)->tail = newElement;
//	} else {
//		(*list)->tail->next = newElement;
//		(*list)->tail = newElement;
//	}
//
//	(*list)->size++;
//}
//
//void* findInFatForwardList(fat_forward_list** list, comparator c) {
//	return findInForwardList(&(*list)->list, c);
//}
//
//bool removeFindInFatForwardList(fat_forward_list** list, comparator c) {
//	//TODO continue to convert from here
//	forward_list* prevComponent = NULL;
//	forward_cell* tmp = *list;
//
//	while (tmp != NULL) {
//		if (c(tmp->pointer) == true) {
//			if (prevComponent == NULL) {
//				//we're removing the head
//				*list = tmp->next;
//			} else {
//				prevComponent->next = tmp->next;
//			}
//			free(tmp);
//			return true;
//		}
//		prevComponent = tmp;
//		tmp = tmp->next;
//	}
//	return false;
//}
//
//bool removeFindWithElementInForwardList(forward_list** list, comparator c, destructor d) {
//	forward_list* prevComponent = NULL;
//	forward_cell* tmp = *list;
//
//	while (tmp != NULL) {
//		if (c(tmp->pointer) == true) {
//			if (prevComponent == NULL) {
//				//we're removing the head
//				*list = tmp->next;
//			} else {
//				prevComponent->next = tmp->next;
//			}
//			d(tmp->pointer);
//			free(tmp);
//			return true;
//		}
//		prevComponent = tmp;
//		tmp = tmp->next;
//	}
//	return false;
//}
//
//void* popHeadFromForwardList(forward_list** list) {
//	if (isForwardListEmpty(*list)) {
//		return NULL;
//	}
//	forward_list* tmp = (*list);
//
//	void* retVal = (*list)->pointer;
//	*list = (*list)->next;
//	free(tmp);
//	return retVal;
//}
//
//void* peekHeadFromForwardList(const forward_list** list) {
//	return isForwardListEmpty(*list) ? NULL : (*list)->pointer;
//}
//
//bool isForwardListEmpty(const forward_list** list) {
//	return (*list) == NULL;
//}
//
//int getForwardListSize(const forward_list** list) {
//	forward_list* tmp = *list;
//	int retVal = 0;
//	while (tmp != NULL) {
//		retVal++;
//		tmp = tmp->next;
//	}
//	return retVal;
//}
//
//void* getTailInForwardList(const forward_list** list) {
//	forward_cell* tail = getTailCellInForwardList(list);
//	return tail != NULL ? tail->pointer : NULL;
//}
//
//void appendForwardListToTail(forward_list** dest, const forward_list** src) {
//	forward_list* destTail = getTailCellInForwardList(*dest);
//	forward_list* tmp = src;
//
//	destTail->next = src;
//}
//
//void clearForwardList(forward_list** toClear) {
//	forward_list* tmp = *toClear;
//	forward_list* tmp2 = NULL;
//	while (tmp != NULL) {
//		tmp2 = tmp->next;
//		free(tmp);
//		tmp = tmp2;
//	}
//	*toClear = NULL;
//}
//
//void clearForwardListWithElements(forward_list** toClear, destructor d) {
//	forward_list* tmp = *toClear;
//	forward_list* tmp2 = NULL;
//	while (tmp != NULL) {
//		tmp2 = tmp->next;
//		d(tmp->pointer);
//		free(tmp);
//		tmp = tmp2;
//	}
//	*toClear = NULL;
//}
//
//static forward_cell* getTailCellInForwardList(const forward_list** list) {
//	forward_list* tmp = *list;
//
//	if (tmp == NULL) {
//		return NULL;
//	}
//	while (true) {
//		if (tmp->next == NULL) {
//			return tmp;
//		}
//		tmp = tmp->next;
//	}
//}
