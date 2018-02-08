/*
 * list.c
 *
 *  Created on: May 17, 2017
 *      Author: koldar
 */

#define _GNU_SOURCE
#include "list.h"
#include "macros.h"
#include "log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "random_utils.h"


list* initList() {
	list* retVal = malloc(sizeof(list));
	if (retVal == NULL) {
		ERROR_MALLOC();
	}

	retVal->head = NULL;
	retVal->size = 0;
	retVal->tail = NULL;

	return retVal;
}

void destroyList(list* lst) {
	ITERATE_ON_LIST(lst, cell, value, void) {
		free(cell);
	}
	free(lst);
}

void destroyListWithElement(list* lst, destructor d) {
	ITERATE_ON_LIST(lst, cell, value, void) {
		debug("deleting payload!!!!");
		d(cell->payload);
		free(cell);
	}
	free(lst);
}

void clearList(list* l) {
	ITERATE_ON_LIST(l, cell, value, void) {
		free(cell);
	}
	l->head = NULL;
	l->size = 0;
	l->tail = NULL;
}

void clearListWithElements(list* l, destructor d) {
	ITERATE_ON_LIST(l, cell, value, void) {
		d(cell->payload);
		free(cell);
	}

	l->head = NULL;
	l->size = 0;
	l->tail = NULL;
}

list* cloneListWithElements(const list* l, cloner c) {
	list* retVal = initList();
	ITERATE_ON_LIST(l, cell, payload, void) {
		addTailInList(retVal, c(payload));
	}
	return retVal;
}

list* cloneList(const list* l) {
	list* retVal = initList();
	ITERATE_ON_LIST(l, cell, payload, void) {
		addTailInList(retVal, payload);
	}
	return retVal;
}

void addHeadInList(list* l, void* el) {
	list_cell* new_cell = malloc(sizeof(list_cell));
	if (new_cell == NULL) {
		ERROR_MALLOC();
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
		ERROR_MALLOC();
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

void* peekFromList(const list* l) {
	return getHeadOfList(l);
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

void* findElementInList(const list* l, finder f) {
	ITERATE_ON_LIST(l, cell, value, void) {
		if (f(value)) {
			return value;
		}
	}
	return NULL;
}

bool removeElementInList(list* l, finder f) {
	bool removed = false;
	VARIABLE_ITERATE_ON_LIST(l, prevCell, cell, payload, void*) {
		if (f(payload)) {
			removeElementInListCell(l, &prevCell, cell);
			removed = true;
			break;
		}
	}

	return removed;
}

bool removeAndDestroyElementInList(list* l, finder f, destructor d) {
	bool removed = false;
	VARIABLE_ITERATE_ON_LIST(l, prevCell, cell, payload, void*) {
		if (f(payload)) {
			d(payload);
			removeElementInListCell(l, &prevCell, cell);
			removed = true;
			break;
		}
	}

	return removed;
}

bool containsItemInListByReference(list* l, void* obj) {
	ITERATE_ON_LIST(l, cell, payload, void) {
		if (payload == obj) {
			return true;
		}
	}
	return false;
}

bool removeItemInListByReference(list* l, void* obj) {
	bool removed = false;
	VARIABLE_ITERATE_ON_LIST(l, prevCell, cell, payload, void*) {
		if (payload == obj) {
			removeElementInListCell(l, &prevCell, cell);
			removed = true;
			break;
		}
	}

	return removed;
}

const char* getStringOfList(const list* l, stringify f) {
	char buffer[BUFFER_SIZE];
	char* buffer2;
	buffer[0] = '\0';
	strcat(buffer, "[");
	ITERATE_ON_LIST(l, cell, payload, void) {
		buffer2 = f(payload);

		if (buffer2 != NULL) {
			strcat(buffer, buffer2);
			//if there is still other data in the list we append also a ","
			if (cell->next != NULL) {
				strcat(buffer, ", ");
			}
		}
		//		sprintf(buffer2, "%s%s", buffer2 != NULL ? buffer2 : "", cell->next != NULL ? ", " : "");
		//		strcat(buffer, buffer2);

		if (buffer2 != NULL) {
			free(buffer2);
		}
	}
	strcat(buffer, "]");
	return strdup(buffer);
}

int getBufferStringOfList(const list* lst, char* buffer, buffer_string f) {
	int i = 0;
	int j = 0;
	i += sprintf(&buffer[i], "[");
	ITERATE_ON_LIST(lst, cell, payload, void) {
		j++;
		i += sprintf(&buffer[i], "%d:(", j);
		i += f(payload, &buffer[i]);
		i += sprintf(&buffer[i], ")");

		if ((cell->next) != NULL) {
			i += sprintf(&buffer[i], ",");
		}
	}
	i += sprintf(&buffer[i], "]");
	return i;
}

void* getNthElementOfList(const list* l, int index) {
	ITERATE_ON_LIST(l, cell, payload, void) {
		if (index == 0) {
			return payload;
		}
		index--;
	}
	return NULL;
}

bool removeNthElementOfList(list* lst, int index) {
	if (index >= getLengthOfList(lst)) {
		return false;
	}

	VARIABLE_ITERATE_ON_LIST(lst, prevCell, cell, payload, void) {
		if (index == 0) {
			removeElementInListCell(lst, &prevCell, cell);
			return true;
		}
		index--;
	}

	return false;
}

void removeElementInListCell(list* l,list_cell** restrict previousCell, list_cell* restrict cellToRemove) {
	list* lst = l;
	list_cell* previous = *previousCell;

	if (previous == NULL) {
		//we're removing the head
		info("removing head");
		popFromList(l);
	} else if (cellToRemove->next == NULL){
		//we're removing the tail
		info("removing tail");
		previous->next = NULL;
		lst->size--;
		lst->tail = previous;
		free(cellToRemove);
	} else {
		info("removing middle element %p %p", previous, cellToRemove);
		info("nexts are %p %p", previous->next, cellToRemove->next);
		//we're removing an element inside the list
		previous->next = cellToRemove->next;
		lst->size--;
		free(cellToRemove);
	}

	*previousCell = NULL;
}

void scrambleList(list* l) {
	//to scramble the list, we apply a bubble sort on the list with a random comparator
	list_cell* cell2;
	ITERATE_ON_LIST(l, cell1, payload1, void) {
		if (cell1->next == NULL) {
			return;
		}
		cell2 = cell1->next;

		while (cell2 != NULL) {
			if ((rand() % 2) == 0) {
				SWAP(cell1->payload, cell2->payload, void*);
			}
			cell2 = cell2->next;
		}
	}
}

void sortList(list* l, orderer order) {
	if (getLengthOfList(l) < 2) {
		return;
	}

	list_cell* cell1 = l->head;
	list_cell* cell2 = NULL;;

	while (cell1 != NULL) {
		if (cell1->next == NULL) {
			return;
		}
		cell2 = cell1->next;

		while (cell2 != NULL) {
			if (order(cell1->payload, cell2->payload) > 0) {
				SWAP(cell1->payload, cell2->payload, void*);
			}
			cell2 = cell2->next;
		}

		cell1 = cell1->next;
	}
}

void* pickRandomItemInList(const list* l) {
	if (isEmptyList(l)) {
		return NULL;
	}

	int rnd = getRandomInRange(0, getLengthOfList(l), true, false);
	list_cell* cell = l->head;
	while (rnd > 0) {
		cell = cell->next;
		rnd--;
	}

	return cell->payload;
}

void storeListInFile(FILE* f, const list* l, object_serializer serializer) {
	//store length of list
	fwrite(&l->size, sizeof(l->size), 1, f);
	//store payloads
	ITERATE_ON_LIST(l, cell, paylaod, void) {
		serializer(f, paylaod);
	}
}

list* loadListFromFile(FILE* f, object_deserializer deserializer) {
	list* retVal = initList();

	//read length of list
	int size = 0;
	fread(&size, sizeof(size), 1, f);
	//read paylods
	critical("size read is %d", size);
	void* payloadRead;
	for (int i=0; i<size; i++) {
		deserializer(f, &payloadRead);
		addTailInList(retVal, payloadRead);
	}

	return retVal;
}

bool isLastItemInList(const list* l, const list_cell* cell) {
	return cell->next == NULL;
}
