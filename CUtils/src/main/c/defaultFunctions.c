/*
 * defaultFunctions.c
 *
 *  Created on: May 18, 2017
 *      Author: koldar
 */

#ifndef _GNU_SOURCE
#	define _GNU_SOURCE
#endif

#include "defaultFunctions.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "macros.h"
#include "log.h"

// *************************** COMPARATORS *********************************

bool defaultPayloadComparator(void* a, void* b) {
	return a == b;
}

// **************************** DESTRUCTORS ****************************

void defaultPayloadDestructor(void* payload) {
	if (payload != NULL) {
		free(payload);
	}
}

void payloadDestructorNOP(void* payload) {
	return;
}

// ************************* STRINGIFIER (TODO REMOVE) *******************************

const char* defaultPayloadStringify(void* payload) {
	char* retVal;
	asprintf(&retVal, "%p", payload);
	return retVal;
}

const char* integerPointerStringify(int* payload) {
	char* retVal;
	if (payload != NULL) {
		asprintf(&retVal, "%d", *payload);
	} else {
		asprintf(&retVal, "NULL");
	}
	return retVal;
}

const char* integerValueStringify(int* payload) {
	char* retVal;
	asprintf(&retVal, "%d", payload);
	return retVal;
}

// ****************************** ORDERER *********************************

int integerPointerOrderer(const int* a, const int* b) {
	return *a - *b;
}

int integerValueOrderer(const int* a, const int* b) {
	//be careful of the size! pointer algebra deals with the size of the pointed type.
	//if sizeof(int)=2 0x7 - 0x1 will compute 0x05!
	return ((int)a) - ((int)b);
}

int defaultOrderer(const void* a, const void* b) {
	return (int)(a - b);
}

// *********************** CLONER ******************************

void* defaultPayloadCloner(void* payload) {
	if (payload == NULL) {
		return NULL;
	}
	return payload;
}

// ******************* COLORER ******************************

int defaultPayloadColor(void* payload) {
	return 0x00000000;
}

// ******************** BUFFER STRING ***************************

int defaultBufferString(void* payload, char* buffer) {
	int i = 0;
	i = sprintf(buffer, "%p", payload);
	return i;
}

int getBufferStringOfInteger(int j, char* buffer) {
	int i = 0;
	i = sprintf(&buffer[i], "%d", j);
	return i;
}

// ********************** SERIALIZER ****************************

int defaultSerializer(FILE* f, void* payload) {
	fwrite(&payload, sizeof(payload), 1, f);
	return 0;
}

int serializerInt(FILE* f, int n) {
	fwrite(&n, sizeof(n), 1, f);
	return sizeof(n);
}

// ********************** DESERIALIZER ******************************

int defaultDeserializer(FILE* f, void** payload) {
	void* p;
	fread(&p, sizeof(p), 1, f);
	*payload = p;
	return 0;
}

int deserializerInt(FILE* f, int* n) {
	fread(n, sizeof(*n), 1, f);
	return sizeof(*n);
}

// ********************** EVALUATOR ***************************

int defaultEvaluator(void* p) {
	return (int)p;
}

// ******************************* HASH FUNCTIONS **********************************

unsigned long hashString(const char* str) {
		//djb2 hash function
		unsigned long hash = 5381;
		int c;
		while (c = *str++)
			hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

		return hash;
}


