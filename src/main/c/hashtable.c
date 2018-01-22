/*
 * hashtable.c
 *
 *  Created on: Jan 24, 2017
 *      Author: koldar
 */

#include "hashtable.h"
#include "errors.h"

// File-local functions declarations
static ct_hashtable_entry_o* ht_init_entry(const void* data, unsigned long key);
static void ht_destroy_entry(const ct_hashtable_entry_o* entry);

struct ct_hashtable_entry {
	/**
	 * the key of a particular value
	 */
	unsigned long id;
	/**
	 * the value in the hashtable
	 */
	void* data;
	/**
	 * A field required by \c uthash project to correctly work
	 */
	UT_hash_handle hh;

};

struct ct_hashtable {
	/**
	 * **head** of the hashtable
	 *
	 * \c uthash hashtables are retrieved by a particular ::ct_hashtable_entry_o called **head**. This field is exactly that
	 */
	ct_hashtable_entry_o* head;
};

ct_hashtable_o* ct_ht_init() {
	ct_hashtable_o* ret_val = malloc(sizeof(ct_hashtable_o));
	if (ret_val == NULL) {
		MALLOCERRORCALLBACK();
	}

	ret_val->head = NULL;

	return ret_val;
}

int ct_ht_size(const ct_hashtable_o* ht) {
	int ret_val;
	ret_val = HASH_COUNT(ht->head);
	return ret_val;
}

void* ct_ht_get(const ct_hashtable_o* ht, unsigned long key) {
	ct_hashtable_entry_o* tmp;

	HASH_FIND(hh, ht->head, &key, sizeof(unsigned long), tmp);
	if (tmp == NULL) {
		return NULL;
	}
	return tmp->data;
}

bool ct_ht_contains(const ct_hashtable_o* ht, unsigned long key) {
	ct_hashtable_entry_o* tmp;
	HASH_FIND(hh, ht->head, &key, sizeof(unsigned long), tmp);
	return (tmp != NULL);
}

bool ct_ht_put_or_update(ct_hashtable_o* ht, unsigned long key, const void* data) {
	ct_hashtable_entry_o* tmp;

	HASH_FIND(hh, ht->head, &key, sizeof(unsigned long), tmp);
	if (tmp == NULL) {
		ct_ht_put(ht, key, data);
		return true;
	} else {
		tmp->data = (void*) data;
		return false;
	}
}

bool ct_ht_update(ct_hashtable_o* ht, unsigned long key, const void* data) {
	ct_hashtable_entry_o* tmp;

	HASH_FIND(hh, ht->head, &key, sizeof(unsigned long), tmp);
	if (tmp == NULL) {
		return false;
	}
	tmp->data = (void*) data;
	return true;
}

void ct_ht_put(ct_hashtable_o* ht, unsigned long key, const void* data) {
	ct_hashtable_entry_o* add = ht_init_entry(data, key);
	HASH_ADD(hh,ht->head,id, sizeof(unsigned long), add);
}

void ct_ht_destroy(ct_hashtable_o* ht) {
	ct_hashtable_entry_o* s;
	ct_hashtable_entry_o* tmp;

	HASH_ITER(hh, ht->head, s, tmp) {
		HASH_DEL(ht->head, s);
		ht_destroy_entry(s);
	}
}

void ct_ht_destroy_with_elements(ct_hashtable_o* ht, ct_destructor_t d) {
	ct_hashtable_entry_o* s;
	ct_hashtable_entry_o* tmp;

	HASH_ITER(hh, ht->head, s, tmp) {
		HASH_DEL(ht->head, s);
		d(s->data);
		ht_destroy_entry(s);
	}
}

bool ct_ht_remove(ct_hashtable_o* ht, unsigned long key) {
	ct_hashtable_entry_o* tmp;

	HASH_FIND(hh, ht->head, &key, sizeof(unsigned long), tmp);
	if (tmp == NULL) {
		return false;
	}
	HASH_DEL(ht->head, tmp);
	ht_destroy_entry(tmp);
	return true;
}

bool ct_ht_remove_and_destroy(ct_hashtable_o* ht, unsigned long key, ct_destructor_t d) {
	ct_hashtable_entry_o* tmp;

	HASH_FIND(hh, ht->head, &key, sizeof(unsigned long), tmp);
	if (tmp == NULL) {
		return false;
	}
	HASH_DEL(ht->head, tmp);
	d(tmp->data);
	ht_destroy_entry(tmp);
	return true;
}

bool ct_ht_is_empty(const ct_hashtable_o* ht) {
	return ht->head == NULL;
}

void* ct_ht_get_first(const ct_hashtable_o* ht) {
	ITERATE_VALUES_ON_HT(ht, data, void*) {
		return data;
	}
	return NULL;
}

bool ct_ht_swap(ct_hashtable_o* ht, unsigned long key1, unsigned long key2) {
	ct_hashtable_entry_o* tmp1;
	ct_hashtable_entry_o* tmp2;

	HASH_FIND(hh, ht->head, &key1, sizeof(unsigned long), tmp1);
	HASH_FIND(hh, ht->head, &key2, sizeof(unsigned long), tmp2);

	if (tmp1 != NULL && tmp2 != NULL) {
		//they are both inside the hashtable
		void* p1 = tmp1->data;
		void* p2 = tmp2->data;
		HASH_DEL(ht->head, tmp1);
		HASH_DEL(ht->head, tmp2);

		ct_ht_put(ht, key1, p2);
		ct_ht_put(ht, key2, p1);

		return true;

	} else if (tmp1 != NULL) {
		//key2 is not inside the hashtable
		ct_ht_put(ht, key2, tmp1->data);
		HASH_DEL(ht->head, tmp1);

		return true;

	} else if (tmp2 != NULL) {
		//key1 is not inside the hashtable
		ct_ht_put(ht, key1, tmp2->data);
		HASH_DEL(ht->head, tmp2);

		return true;
	}

	return false;
}

void ct_ht_clear(ct_hashtable_o* ht) {
	ct_hashtable_entry_o* s;
		ct_hashtable_entry_o* tmp;

	HASH_ITER(hh, ht->head, s, tmp) {
		HASH_DEL(ht->head, s);
		ht_destroy_entry(s);
	}
	ht->head = NULL;
}

void ct_ht_clear_and_destroy_elements(ct_hashtable_o* ht, ct_destructor_t d) {
	ct_hashtable_entry_o* s;
	ct_hashtable_entry_o* tmp;

	HASH_ITER(hh, ht->head, s, tmp) {
		HASH_DEL(ht->head, s);
		d(s->data);
		ht_destroy_entry(s);
	}
	ht->head = NULL;
}

ct_hashtable_entry_o* _ct_ht_head_entry(ct_hashtable_o* ht) {
	return ht->head;
}

void* _ct_ht_entry_next(ct_hashtable_entry_o* entry) {
	return entry->hh.next;
}

void* _ct_ht_entry_payload(ct_hashtable_entry_o* entry) {
	return entry->data;
}

/**
 *
 * \note
 * The data is added in the cell <b>by reference</b>
 *
 * @param[in] data the data to put inside the cell
 * @param[in] f a function with one parameter <tt>void*</tt> returning the hash of the value, aka an int
 * @return a cell of an hash table. You still need to manually add it in the hash table though
 */
static ct_hashtable_entry_o* ht_init_entry(const void* data, unsigned long key) {
	ct_hashtable_entry_o* ret_val = malloc(sizeof(ct_hashtable_entry_o));
	if (ret_val == NULL) {
		MALLOCERRORCALLBACK();
	}

	ret_val->id = key;
	ret_val->data = (void*) data;

	return ret_val;
}

/**
 * Frees from the memory the structure \c ct_hashtable_entry_o
 *
 * \attention
 * the function <b>won't free the pointer</b> ::ct_hashtable_entry_o::data
 *
 * @param[in] entry the cell to remove from the memory
 */
static void ht_destroy_entry(const ct_hashtable_entry_o* entry) {
	free((void*) entry);
}
