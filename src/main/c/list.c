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
	///represents the payload inside this cell of the list
	void* payload;
	///a pointer to the next element of the list. Can be NULL
	struct list_cell* next;
} ct_list_cell_t;

typedef struct list {
	///number of elements inside the list. Allows count operation to be O(1)
	int size;
	///pointer to the first element of the list. Can be NULL
	ct_list_cell_t* head;
	///pointer to the last element of the list. Can be NULL
	ct_list_cell_t* tail;
} ct_list_t;

ct_list_cell_t* ct_next_list_cell(const ct_list_cell_t* cell) {
	return cell->next;
}

void* ct_list_cell_payload(const ct_list_cell_t* cell) {
	return cell->payload;
}

ct_list_cell_t* ct_list_head_cell(const ct_list_t* l) {
	return l->head;
}

ct_list_t* ct_init_list() {
	ct_list_t* ret_val = malloc(sizeof(ct_list_t));
	if (ret_val == NULL) {
		MALLOCERRORCALLBACK();
	}

	ret_val->head = NULL;
	ret_val->size = 0;
	ret_val->tail = NULL;

	return ret_val;
}

void ct_destroy_list(ct_list_t* lst) {
	ITERATE_ON_LIST(lst, cell, value, void*) {
		free(cell);
	}
	free(lst);
}

void ct_destroy_list_with_elements(ct_list_t* lst, ct_destructor_t d) {
	ITERATE_ON_LIST(lst, cell, value, void*) {
		d(cell->payload);
		free(cell);
	}
	free(lst);
}

void ct_clear_list(ct_list_t* l) {
	ITERATE_ON_LIST(l, cell, value, void*) {
		free(cell);
	}
	l->head = NULL;
	l->size = 0;
	l->tail = NULL;
}

void ct_add_head_in_list(ct_list_t* l, void* el) {
	ct_list_cell_t* new_cell = malloc(sizeof(ct_list_cell_t));
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

void ct_add_tail_in_list(ct_list_t* l, void* el) {
	ct_list_cell_t* new_cell = malloc(sizeof(ct_list_cell_t));
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

int ct_list_length(const ct_list_t* l) {
	return l->size;
}

void ct_move_list_contents(ct_list_t* restrict dst, ct_list_t* restrict src) {
	//*********** DST **********
	dst->size += ct_list_length(src);
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

void* ct_list_last_element(const ct_list_t* l) {
	if (l->tail == NULL) {
		return NULL;
	}
	return l->tail->payload;
}

bool ct_is_list_empty(const ct_list_t* l) {
	return l->size == 0;
}

void* ct_pop_from_list(ct_list_t* l) {
	if (ct_is_list_empty(l)) {
		return NULL;
	}

	ct_list_cell_t* cell = l->head;
	void* retVal = cell->payload;
	l->head = l->head->next;
	l->size--;
	if (ct_is_list_empty(l)) {
		l->tail = NULL;
	}

	free(cell);
	return retVal;
}

void* ct_list_head(const ct_list_t* l) {
	if (ct_is_list_empty(l)) {
		return NULL;
	}
	return l->head->payload;
}

void* ct_list_tail(const ct_list_t* l) {
	if (ct_is_list_empty(l)) {
		return NULL;
	}
	return l->tail->payload;
}

void* ct_get_list_element(const ct_list_t* l, int index) {
	ITERATE_ON_LIST(l, cell, payload, void*) {
		if (index == 0) {
			return payload;
		}
		index--;
	}
	return NULL;
}

void ct_remove_element_list_cell(ct_list_t* l, ct_list_cell_t** restrict previous_cell, ct_list_cell_t* restrict cell_to_remove) {
	ct_list_t* lst = l;
	ct_list_cell_t* previous = *previous_cell;

	if (previous == NULL) {
		//we're removing the head
		ct_pop_from_list(l);
	} else if (cell_to_remove->next == NULL){
		//we're removing the tail
		previous->next = NULL;
		lst->size--;
		lst->tail = previous;
		free(cell_to_remove);
	} else {
		//we're removing an element inside the list
		previous->next = cell_to_remove->next;
		lst->size--;
		free(cell_to_remove);
	}

	*previous_cell = NULL;
}
