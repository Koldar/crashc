/*
 * list.c
 *
 *  Created on: May 17, 2017
 *      Author: koldar
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "macros.h"
#include "errors.h"



typedef struct list_cell {
	///represents the paylaod inside this cell of the list
	void* payload;
	///a pointer to the next element of the list. Can be NULL
	struct list_cell* next;
} list_cell;

typedef struct list {
	///number of elements inside the list. Allows count operation to be O(1)
	int size;
	///pointer to the first element of the list. Can be NULL
	list_cell* head;
	///pointer to the last element of the list. Can be NULL
	list_cell* tail;
} list;

list_cell* getNextInListCell(const list_cell* cell) {
	return cell->next;
}

void* getPayloadInListCell(const list_cell* cell) {
	return cell->payload;
}

list_cell* getHeadInList(const list* l) {
	return l->head;
}

list* initList() {
	list* retVal = malloc(sizeof(list));
	if (retVal == NULL) {
		MALLOCERRORCALLBACK();
	}

	retVal->head = NULL;
	retVal->size = 0;
	retVal->tail = NULL;

	return retVal;
}

void destroyList(list* lst) {
	ITERATE_ON_LIST(lst, cell, value, void*) {
		free(cell);
	}
	free(lst);
}

void destroyListWithElement(list* lst, ct_destructor_t d) {
	ITERATE_ON_LIST(lst, cell, value, void*) {
		d(cell->payload);
		free(cell);
	}
	free(lst);
}

void clearList(list* l) {
	ITERATE_ON_LIST(l, cell, value, void*) {
		free(cell);
	}
	l->head = NULL;
	l->size = 0;
	l->tail = NULL;
}

void addHeadInList(list* l, void* el) {
	list_cell* new_cell = malloc(sizeof(list_cell));
	if (new_cell == NULL) {
		MALLOCERRORCALLBACK();
	}

	new_cell->payload = el;
	new_cell->next = l->head;

	l->size++;
	l->head = new_cell;
	if (l->tail == NULL) {
		l->tail = new_cell;
	}
}

void addTailInList(list* l, void* el) {
	list_cell* new_cell = malloc(sizeof(list_cell));
	if (new_cell == NULL) {
		MALLOCERRORCALLBACK();
	}

	new_cell->payload = el;
	new_cell->next = NULL;
	if (l->tail != NULL) {
		l->tail->next = new_cell;
	}

	l->size++;
	if (l->head == NULL) {
		l->head = new_cell;
	}
	l->tail = new_cell;

}

int getLengthOfList(const list* l) {
	return l->size;
}

void moveListContents(list* restrict dst, list* restrict src) {
	//*********** DST **********
	dst->size += getLengthOfList(src);
	if (dst->head == NULL) {
		dst->head = src->head;
	} else {
		dst->tail->next = src->head;
	}
	dst->tail = src->tail;

	//************ SRC **************
	src->size = 0;
	src->head = NULL;
	src->tail = NULL;
}

void* getLastElementOfList(const list* l) {
	if (l->tail == NULL) {
		return NULL;
	}
	return l->tail->payload;
}

bool isEmptyList(const list* l) {
	return l->size == 0;
}

void* popFromList(list* l) {
	if (isEmptyList(l)) {
		return NULL;
	}

	list_cell* cell = l->head;
	void* retVal = cell->payload;
	l->head = l->head->next;
	l->size--;
	if (isEmptyList(l)) {
		l->tail = NULL;
	}

	free(cell);
	return retVal;
}

void* getHeadOfList(const list* l) {
	if (isEmptyList(l)) {
		return NULL;
	}
	return l->head->payload;
}

void* getTailOfList(const list* l) {
	if (isEmptyList(l)) {
		return NULL;
	}
	return l->tail->payload;
}

void* getNthElementOfList(const list* l, int index) {
	ITERATE_ON_LIST(l, cell, payload, void*) {
		if (index == 0) {
			return payload;
		}
		index--;
	}
	return NULL;
}

void removeElementInListCell(list* l,list_cell** restrict previousCell, list_cell* restrict cellToRemove) {
	list* lst = l;
	list_cell* previous = *previousCell;

	if (previous == NULL) {
		//we're removing the head
		popFromList(l);
	} else if (cellToRemove->next == NULL){
		//we're removing the tail
		previous->next = NULL;
		lst->size--;
		lst->tail = previous;
		free(cellToRemove);
	} else {
		//we're removing an element inside the list
		previous->next = cellToRemove->next;
		lst->size--;
		free(cellToRemove);
	}

	*previousCell = NULL;
}
