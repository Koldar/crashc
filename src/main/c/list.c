/*
 * list.c
 *
 *  Created on: May 17, 2017
 *      Author: koldar
 */

#include <stdio.h>
#include <string.h>

#include "list.h"
#include "macros.h"
#include "errors.h"


struct ct_list_entry {
	///represents the payload inside this cell of the list
	void* payload;
	///a pointer to the next element of the list. Can be NULL
	struct ct_list_entry* next;
};

struct ct_list {
	///number of elements inside the list. Allows count operation to be O(1)
	int size;
	///pointer to the first element of the list. Can be NULL
	ct_list_entry_o* head;
	///pointer to the last element of the list. Can be NULL
	ct_list_entry_o* tail;
};

ct_list_entry_o* _ct_list_get_next_entry(const ct_list_entry_o* entry) {
	return entry->next;
}

void* _ct_list_get_entry_payload(const ct_list_entry_o* entry) {
	return entry->payload;
}

ct_list_entry_o* _ct_list_head_entry(const ct_list_o* l) {
	return l->head;
}

ct_list_o* ct_list_init() {
	ct_list_o* ret_val = malloc(sizeof(ct_list_o));
	if (ret_val == NULL) {
		MALLOCERRORCALLBACK();
	}

	ret_val->head = NULL;
	ret_val->size = 0;
	ret_val->tail = NULL;

	return ret_val;
}

void ct_list_destroy(const ct_list_o* lst) {
	ITERATE_ON_LIST(lst, entry, value, void*) {
		free(entry);
	}
	free((void*) lst);
}

void ct_list_destroy_with_elements(const ct_list_o* lst, ct_destroyer_c d) {
	ITERATE_ON_LIST(lst, entry, value, void*) {
		d(entry->payload);
		free(entry);
	}
	free((void*) lst);
}

void ct_list_clear(ct_list_o* l) {
	ITERATE_ON_LIST(l, entry, value, void*) {
		free(entry);
	}
	l->head = NULL;
	l->size = 0;
	l->tail = NULL;
}

void ct_list_add_head(ct_list_o* l, const void* el) {
	ct_list_entry_o* new_entry = malloc(sizeof(ct_list_entry_o));
	if (new_entry == NULL) {
		MALLOCERRORCALLBACK();
	}

	new_entry->payload = (void*)el;
	new_entry->next = l->head;

	l->size++;
	l->head = new_entry;
	if (l->tail == NULL) {
		l->tail = new_entry;
	}
}

void ct_list_add_tail(ct_list_o* l, const void* el) {
	ct_list_entry_o* new_entry = malloc(sizeof(ct_list_entry_o));
	if (new_entry == NULL) {
		MALLOCERRORCALLBACK();
	}

	new_entry->payload = (void*)el;
	new_entry->next = NULL;
	if (l->tail != NULL) {
		l->tail->next = new_entry;
	}

	l->size++;
	if (l->head == NULL) {
		l->head = new_entry;
	}
	l->tail = new_entry;

}

int ct_list_size(const ct_list_o* l) {
	return l->size;
}

void ct_list_full_transfer(ct_list_o* restrict dst, ct_list_o* restrict src) {
	//*********** DST **********
	dst->size += ct_list_size(src);
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

bool ct_list_is_empty(const ct_list_o* l) {
	return l->size == 0;
}

void* ct_list_pop(ct_list_o* l) {
	if (ct_list_is_empty(l)) {
		return NULL;
	}

	ct_list_entry_o* cell = l->head;
	void* ret_val = cell->payload;
	l->head = l->head->next;
	l->size--;
	if (ct_list_is_empty(l)) {
		l->tail = NULL;
	}

	free(cell);
	return ret_val;
}

void* ct_list_head(const ct_list_o* l) {
	if (ct_list_is_empty(l)) {
		return NULL;
	}
	return l->head->payload;
}

void* ct_list_tail(const ct_list_o* l) {
	if (ct_list_is_empty(l)) {
		return NULL;
	}
	return l->tail->payload;
}

void* ct_list_get(const ct_list_o* l, int index) {
	ITERATE_ON_LIST(l, entry, payload, void*) {
		if (index == 0) {
			return payload;
		}
		index--;
	}
	return NULL;
}

void ct_list_remove_entry_dynamic(ct_list_o* l, ct_list_entry_o** previous_entry, ct_list_entry_o* entry_to_remove) {
	ct_list_o* lst = l;
	ct_list_entry_o* previous = *previous_entry;

	if (previous == NULL) {
		//we're removing the head
		ct_list_pop(l);
	} else if (entry_to_remove->next == NULL){
		//we're removing the tail
		previous->next = NULL;
		lst->size--;
		lst->tail = previous;
		free(entry_to_remove);
	} else {
		//we're removing an element inside the list
		previous->next = entry_to_remove->next;
		lst->size--;
		free(entry_to_remove);
	}

	*previous_entry = NULL;
}
