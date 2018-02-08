/*
 * hashtable.c
 *
 *  Created on: Jan 24, 2017
 *      Author: koldar
 */

#include "hashtable.h"
#include "macros.h"
#include <stdbool.h>

static HTCell* initHTCell(const void* e, unsigned long key);
static void destroyHTCell(HTCell* htCell);

HT* initHT() {
	return NULL;
}

int getSizeOfHT(const HT** ht) {
	int retVal;
	retVal = HASH_COUNT(*ht);
	return retVal;
}

void* getItemInHT(const HT** ht, unsigned long key) {
	HTCell* tmp;

	HASH_FIND(hh, *ht, &key, sizeof(unsigned long), tmp);
	if (tmp == NULL) {
		return NULL;
	}
	return tmp->data;
}

bool containsItemInHT(const HT** ht, unsigned long key) {
	HTCell* tmp;
	HASH_FIND(hh, *ht, &key, sizeof(unsigned long), tmp);
	return (tmp != NULL);
}

void addItemInHT(HT** ht, void* data, hashFunction f) {
	addItemInHTWithKey(ht, f(data), data);
}

bool addOrUpdateItemInHT(HT** ht, unsigned long key, void* data) {
	HTCell* tmp;

	HASH_FIND(hh, *ht, &key, sizeof(unsigned long), tmp);
	if (tmp == NULL) {
		addItemInHTWithKey(ht, key, data);
		return true;
	} else {
		tmp->data = data;
		return false;
	}
}

bool updateItemInHT(HT** ht, unsigned long key, void* data) {
	HTCell* tmp;

	HASH_FIND(hh, *ht, &key, sizeof(unsigned long), tmp);
	if (tmp == NULL) {
		return false;
	}
	tmp->data = data;
	return true;
}

void addItemInHTWithKey(HT** ht, unsigned long key, void* data) {
	HTCell* add = initHTCell(data, key);
	HASH_ADD(hh,*ht,id, sizeof(unsigned long), add);
}

void destroyHT(HT** ht) {
	HTCell* s;
	HTCell* tmp;

	HASH_ITER(hh, *ht, s, tmp) {
		HASH_DEL(*ht, s);
		destroyHTCell(s);
	}
}

void destroyHTWithElements(HT** ht, destructor d) {
	HTCell* s;
	HTCell* tmp;

	HASH_ITER(hh, *ht, s, tmp) {
		HASH_DEL(*ht, s);
		d(s->data);
		destroyHTCell(s);
	}
}


void deleteHTCell(HT** ht, HTCell* htCell) {
	HASH_DEL(*ht, htCell);
	destroyHTCell(htCell);
}

void destroyHTCellWithElement(HTCell* htCell, destructor d) {
	d(htCell->data);
	free(htCell);
}

bool deleteItemInHT(HT** ht, unsigned long key) {
	HTCell* tmp;

	HASH_FIND(hh, *ht, &key, sizeof(unsigned long), tmp);
	if (tmp == NULL) {
		return false;
	}
	HASH_DEL(*ht, tmp);
	destroyHTCell(tmp);
	return true;
}

bool deleteItemInHTWithElement(HT** ht, unsigned long key, destructor d) {
	HTCell* tmp;

	HASH_FIND(hh, *ht, &key, sizeof(unsigned long), tmp);
	if (tmp == NULL) {
		return false;
	}
	HASH_DEL(*ht, tmp);
	d(tmp->data);
	destroyHTCell(tmp);
	return true;
}

bool isHTEmpty(HT** ht) {
	return *ht == NULL;
}

void* findItemInHT(const HT** ht, finder finder) {
	HTCell* el;
	HTCell* tmp;
	HASH_ITER(hh, *ht, el, tmp) {
		if (finder(el->data)){
			return el->data;
		}
	}
	return NULL;
}

bool isHTContainingItems(const HT** ht, finder finder) {
	return findItemInHT(ht, finder) != NULL;
}

void* getFirstItemInHT(const HT** ht) {
	ITERATE_VALUES_ON_HT(ht, data, void) {
		return data;
	}
	return NULL;
}

bool swapValuesInHT(HT** ht, unsigned long key1, unsigned long key2) {
	HTCell* tmp1;
	HTCell* tmp2;

	HASH_FIND(hh, *ht, &key1, sizeof(unsigned long), tmp1);
	HASH_FIND(hh, *ht, &key2, sizeof(unsigned long), tmp2);

	if (tmp1 != NULL && tmp2 != NULL) {
		//they are both inside the hashtable
		void* p1 = tmp1->data;
		void* p2 = tmp2->data;
		HASH_DEL(*ht, tmp1);
		HASH_DEL(*ht, tmp2);

		addItemInHTWithKey(*ht, key1, p2);
		addItemInHTWithKey(*ht, key2, p1);
		return true;
	} else if (tmp1 != NULL) {
		//key2 is not inside the hashtable
		addItemInHTWithKey(ht, key2, tmp1->data);
		HASH_DEL(*ht, tmp1);
		return true;
	} else if (tmp2 != NULL) {
		//key1 is not inside the hashtable
		addItemInHTWithKey(ht, key1, tmp2->data);
		HASH_DEL(*ht, tmp2);
		return true;
	}

	return false;
}

void storeHTInFile(FILE* f, const HT** ht, object_serializer payload) {
	char buffer[BUFFER_SIZE];
	int size;
	int htSize = getSizeOfHT(ht);
	fwrite(&htSize, sizeof(htSize), 1, f);
	ITERATE_ON_HT(ht, el) {
		fwrite(&el->id, sizeof(el->id), 1, f);
		payload(f, el->data);
	}
}

HT* loadHTFromFile(FILE* f, object_deserializer payload) {
	HT* retVal = initHT();
	int size;
	unsigned long key;
	void* data;
	fread(&size, sizeof(size), 1, f);
	for(int i=0; i<size; i++) {
		fread(&key, sizeof(key), 1, f);
		payload(f, &data);
		addItemInHTWithKey(&retVal, key, data);
	}
	return retVal;
}

void clearHT(HT** ht) {
	HTCell* s;
		HTCell* tmp;

	HASH_ITER(hh, *ht, s, tmp) {
		HASH_DEL(*ht, s);
		destroyHTCell(s);
	}
	*ht = NULL;
}

void clearHTWithElements(HT** ht, destructor d) {
	HTCell* s;
	HTCell* tmp;

	HASH_ITER(hh, *ht, s, tmp) {
		HASH_DEL(*ht, s);
		d(s->data);
		destroyHTCell(s);
	}
	*ht = NULL;
}

int getBufferStringOfHT(const HT** ht, char* buffer, buffer_string stringer) {
	int i = 0;
	int j = 0;
	void* payload;
	i += sprintf(&buffer[i], "{");
	ITERATE_ON_HT(ht, cell) {
		j++;
		i += sprintf(&buffer[i], "<%ld: ", cell->id);
		i += stringer(cell->data, &buffer[i]);
		i += sprintf(&buffer[i], ">");

		if ((cell->hh.next) != NULL) {
			i += sprintf(&buffer[i], ",");
		}
	}
	i += sprintf(&buffer[i], "}");
	return i;
}

HT* cloneHT(const HT** ht) {
	HT* retVal = initHT();

	ITERATE_ON_HT(ht, el) {
		addItemInHTWithKey(&retVal, el->id, el->data);
	}

	return retVal;
}

HT* cloneHTWithElements(const HT** ht, cloner c) {
	HT* retVal = initHT();

	ITERATE_ON_HT(ht, el) {
		addItemInHTWithKey(&retVal, el->id, c(el->data));
	}

	return retVal;
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
		ERROR_MALLOC();
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
