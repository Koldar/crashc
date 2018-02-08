/*
 * tuple.c
 *
 *  Created on: Jan 22, 2018
 *      Author: koldar
 */

#include "tuple.h"
#include "macros.h"
#include <stdlib.h>

pointer_pair_t* initPointerPair(const void* first, const void* second) {
	pointer_pair_t* retVal = malloc(sizeof(pointer_pair_t));
	if (retVal == NULL) {
		ERROR_MALLOC();
	}

	retVal->first = first;
	retVal->second = second;

	return retVal;
}

void destroyPointerPair(const pointer_pair_t* p) {
	free(p);
}

void destroyPointerPairWithElements(const pointer_pair_t* p, destructor dFirst, destructor dSecond) {
	dFirst(p->first);
	dSecond(p->second);
	free(p);
}
