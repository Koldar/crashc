/*
 * hashtable.c
 *
 *  Created on: Jan 24, 2017
 *      Author: koldar
 */

#include <stdbool.h>

#include "hashtable.h"
#include "macros.h"
#include "errors.h"


static HTCell* initHTCell(const void* e, unsigned long key);
static void destroyHTCell(HTCell* htCell);

HT* initHT() {
	HT* retVal = malloc(sizeof(HT));
	if (retVal == NULL) {
		MALLOCERRORCALLBACK();
	}

	retVal->head = NULL;

	return retVal;
}

int getSizeOfHT(const HT* ht) {
	int retVal;
	retVal = HASH_COUNT(ht->head);
	return retVal;
}

void* getItemInHT(const HT* ht, unsigned long key) {
	HTCell* tmp;

	HASH_FIND(hh, ht->head, &key, sizeof(unsigned long), tmp);
	if (tmp == NULL) {
		return NULL;
	}
	return tmp->data;
}

bool containsItemInHT(const HT* ht, unsigned long key) {
	HTCell* tmp;
	HASH_FIND(hh, ht->head, &key, sizeof(unsigned long), tmp);
	return (tmp != NULL);
}

bool addOrUpdateItemInHT(HT* ht, unsigned long key, void* data) {
	HTCell* tmp;

	HASH_FIND(hh, ht->head, &key, sizeof(unsigned long), tmp);
	if (tmp == NULL) {
		addItemInHTWithKey(ht, key, data);
		return true;
	} else {
		tmp->data = data;
		return false;
	}
}

bool updateItemInHT(HT* ht, unsigned long key, void* data) {
	HTCell* tmp;

	HASH_FIND(hh, ht->head, &key, sizeof(unsigned long), tmp);
	if (tmp == NULL) {
		return false;
	}
	tmp->data = data;
	return true;
}

void addItemInHTWithKey(HT* ht, unsigned long key, void* data) {
	HTCell* add = initHTCell(data, key);
	HASH_ADD(hh,ht->head,id, sizeof(unsigned long), add);
}

void destroyHT(HT* ht) {
	HTCell* s;
	HTCell* tmp;

	HASH_ITER(hh, ht->head, s, tmp) {
		HASH_DEL(ht->head, s);
		destroyHTCell(s);
	}
}

void destroyHTWithElements(HT* ht, void(*destructor)(void*)) {
	HTCell* s;
	HTCell* tmp;

	HASH_ITER(hh, ht->head, s, tmp) {
		HASH_DEL(ht->head, s);
		destructor(s->data);
		destroyHTCell(s);
	}
}


void deleteHTCell(HT* ht, HTCell* htCell) {
	HASH_DEL(ht->head, htCell);
	destroyHTCell(htCell);
}

void destroyHTCellWithElement(HTCell* htCell, void(*destructor)(void*)) {
	destructor(htCell->data);
	free(htCell);
}

bool deleteItemInHT(HT* ht, unsigned long key) {
	HTCell* tmp;

	HASH_FIND(hh, ht->head, &key, sizeof(unsigned long), tmp);
	if (tmp == NULL) {
		return false;
	}
	HASH_DEL(ht->head, tmp);
	destroyHTCell(tmp);
	return true;
}

bool deleteItemInHTWithElement(HT* ht, unsigned long key, void(*destructor)(void*)) {
	HTCell* tmp;

	HASH_FIND(hh, ht->head, &key, sizeof(unsigned long), tmp);
	if (tmp == NULL) {
		return false;
	}
	HASH_DEL(ht->head, tmp);
	destructor(tmp->data);
	destroyHTCell(tmp);
	return true;
}

bool isHTEmpty(const HT* ht) {
	return ht->head == NULL;
}

void* getFirstItemInHT(const HT* ht) {
	ITERATE_VALUES_ON_HT(ht, data, void*) {
		return data;
	}
	return NULL;
}

bool swapValuesInHT(HT* ht, unsigned long key1, unsigned long key2) {
	HTCell* tmp1;
	HTCell* tmp2;

	HASH_FIND(hh, ht->head, &key1, sizeof(unsigned long), tmp1);
	HASH_FIND(hh, ht->head, &key2, sizeof(unsigned long), tmp2);

	if (tmp1 != NULL && tmp2 != NULL) {
		//they are both inside the hashtable
		void* p1 = tmp1->data;
		void* p2 = tmp2->data;
		HASH_DEL(ht->head, tmp1);
		HASH_DEL(ht->head, tmp2);

		addItemInHTWithKey(ht, key1, p2);
		addItemInHTWithKey(ht, key2, p1);
		return true;
	} else if (tmp1 != NULL) {
		//key2 is not inside the hashtable
		addItemInHTWithKey(ht, key2, tmp1->data);
		HASH_DEL(ht->head, tmp1);
		return true;
	} else if (tmp2 != NULL) {
		//key1 is not inside the hashtable
		addItemInHTWithKey(ht, key1, tmp2->data);
		HASH_DEL(ht->head, tmp2);
		return true;
	}

	return false;
}

void clearHT(HT* ht) {
	HTCell* s;
		HTCell* tmp;

	HASH_ITER(hh, ht->head, s, tmp) {
		HASH_DEL(ht->head, s);
		destroyHTCell(s);
	}
	ht->head = NULL;
}

void clearHTWithElements(HT* ht, void(*destructor)(void*)) {
	HTCell* s;
	HTCell* tmp;

	HASH_ITER(hh, ht->head, s, tmp) {
		HASH_DEL(ht->head, s);
		destructor(s->data);
		destroyHTCell(s);
	}
	ht->head = NULL;
}

/**
 *
 * \note
 * The data is added in the cell <b>by reference</b>
 *
 * @param[in] e the data to put inside the cell
 * @param[in] f a function with one parameter <tt>void*</tt> returning the hash of the value, aka an int
 * @return a cell of an hash table. You still need to manually add it in the hash table though
 */
static HTCell* initHTCell(const void* e, unsigned long key) {
	HTCell* retVal = malloc(sizeof(HTCell));
	if (retVal == NULL) {
		MALLOCERRORCALLBACK();
	}

	retVal->id = key;
	retVal->data = e;

	return retVal;
}

/**
 * Frees from the memory the structure \c htCell
 *
 * \attention
 * the function <b>won't free the pointer</b> ::HTCell::data
 *
 * @param[in] htCell the cell to remove from the memory
 */
static void destroyHTCell(HTCell* htCell) {
	free(htCell);
}
