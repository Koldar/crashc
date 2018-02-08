/*
 * hash_set.c
 *
 *  Created on: Dec 21, 2017
 *      Author: koldar
 */

#include "hash_set.h"
#include "hashtable.h"
#include "macros.h"
#include "log.h"

struct hash_set {
	HT* hashTable;
	payload_functions functions;
};


hash_set* initHashSet(payload_functions functions) {
	hash_set* retVal = malloc(sizeof(hash_set));
	if (retVal == NULL) {
		ERROR_MALLOC();
	}

	retVal->hashTable = initHT();
	retVal->functions = functions;

	return retVal;
}

bool addItemInHashSet(hash_set* set, const void* item) {
	return addOrUpdateItemInHT(&set->hashTable, item, item);
}

bool containsItemInHashSet(const hash_set* set, const void* item) {
	return containsItemInHT(&set->hashTable, item);
}

void removeItemInHashSet(hash_set* set, const void* item) {
	deleteItemInHT(&set->hashTable, item);
}

void destroyHashSet(hash_set* set) {
	destroyHT(&set->hashTable);
	free(set);
}

void destroyHashSetWithElements(hash_set* set) {
	destroyHTWithElements(&set->hashTable, set->functions.destroy);
	free(set);
}

size_t getSizeOfHashSet(const hash_set* set) {
	return getSizeOfHT(&set->hashTable);
}

bool isHashSetEmpty(const hash_set* set) {
	return isHTEmpty(&set->hashTable);
}

hash_set* getUnionOfHashSets(hash_set* restrict set1, const hash_set* restrict set2, bool inPlace) {
	hash_set* retVal;

	if (inPlace) {
		retVal = set1;

		ITERATE_OVER_HASHSET(set2, var, void*) {
			if (!containsItemInHashSet(retVal, var)) {
				addItemInHashSet(retVal, var);
			}
		}
	} else {
		hash_set* minSizedSet = getSizeOfHashSet(set1) < getSizeOfHashSet(set2) ? set1 : set2;
		hash_set* maxSizedSet = getSizeOfHashSet(set1) >= getSizeOfHashSet(set2) ? set1 : set2;

		retVal = cloneHashSetByReference(maxSizedSet);

		ITERATE_OVER_HASHSET(minSizedSet, var, void*) {
			if (!containsItemInHashSet(retVal, var)) {
				addItemInHashSet(retVal, var);
			}
		}
	}

	return retVal;
}


hash_set* getIntersectionOfHashSets(hash_set* restrict set1, const hash_set* restrict set2, bool inPlace) {
	hash_set* retVal;

	if (inPlace) {
		retVal = set1;

		VARIABLE_ITERATE_OVER_HASHSET(retVal, var, void*) {
			if (!containsItemInHashSet(set2, var)) {
				removeItemInHashSet(retVal, var);
			}
		}
	} else {
		retVal = initHashSet(set1->functions);

		//the ">=" handle the scenario where the sets have the same size
		hash_set* minSizedSet = getSizeOfHashSet(set1) < getSizeOfHashSet(set2) ? set1 : set2;
		hash_set* maxSizedSet = getSizeOfHashSet(set1) >= getSizeOfHashSet(set2) ? set1 : set2;

		VARIABLE_ITERATE_OVER_HASHSET(minSizedSet, var, void*) {
			if (containsItemInHashSet(maxSizedSet, var)) {
				addItemInHashSet(retVal, var);
			}
		}
	}

	return retVal;
}

hash_set* getDifferenceOfHashSets(hash_set* restrict set1, const hash_set* restrict set2, bool inPlace) {
	hash_set* retVal;

	if (inPlace) {
		retVal = set1;
	} else {
		retVal = cloneHashSetByReference(set1);
	}

	ITERATE_OVER_HASHSET(retVal, var, void*) {
		if (containsItemInHashSet(set2, var)) {
			removeItemInHashSet(retVal, var);
			if (isHashSetEmpty(retVal)) {
				return retVal;
			}
		}
	}

	return retVal;
}

void clearHashSet(const hash_set* set) {
	clearHT(&set->hashTable);
}

void clearHashSetWithElements(const hash_set* set) {
	clearHTWithElements(&set->hashTable, set->functions.destroy);
}

int getBufferStringOfHashSet(const hash_set* set, char* buffer) {
	int i=0;

	i += sprintf(&buffer[i], "{");

	int count = 0;
	ITERATE_OVER_HASHSET(set, var, void*) {
		count++;
		i += set->functions.bufferString(var, &buffer[i]);
		if (count != getSizeOfHashSet(set)) {
			i += sprintf(&buffer[i], ", ");
		}
	}

	i += sprintf(&buffer[i], "}");

	return i;
}

hash_set* cloneHashSetByReference(const hash_set* set) {
	hash_set* retVal = initHashSet(set->functions);

	ITERATE_OVER_HASHSET(set, var, void*) {
		addItemInHashSet(retVal, var);
	}

	return retVal;
}

hash_set* cloneHashSet(const hash_set* set) {
	hash_set* retVal = initHashSet(set->functions);

	ITERATE_OVER_HASHSET(set, var, void*) {
		addItemInHashSet(retVal, set->functions.clone(var));
	}

	return retVal;
}

HT** _getUnderlyingHashTableInHashSet(const hash_set* set) {
	return &set->hashTable;
}
