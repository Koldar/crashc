/*
 * \file defaultFunctions.h
 *
 * Contains some functions used inside the global standard functions like ::edgePayloadComparatorGlobal
 *
 * \see \ref globalVariables
 *
 *  Created on: May 18, 2017
 *      Author: koldar
 */

#ifndef DEFAULTFUNCTIONS_H_
#define DEFAULTFUNCTIONS_H_

#include <stdbool.h>
#include <stdio.h>


/**
 * Compare the 2 pointers by reference
 *
 * \ingroup comparator
 */
bool defaultPayloadComparator(void* a, void* b);

/**
 * Free the pointer \c payload
 *
 * \ingroup destructor
 */
void defaultPayloadDestructor(void* payload);

/**
 * prints the pointer of the \c payload
 *
 * \ingroup stringifier
 */
const char* defaultPayloadStringify(void* payload);

/**
 * Get the string representation of an \c int variable
 *
 * the variable is stored in the address pointed by \c payload
 *
 * \ingroup stringifier
 */
const char* integerPointerStringify(int* payload);

/**
 * Decide which integer is less than the other
 *
 * the variable is stored in the address pointed by \c a and \c b
 *
 * \ingroup orderer
 */
int integerPointerOrderer(const int* a, const int* b);

/**
 * Get the string representation of an \c int variable
 *
 * the variable is stored in the pointer \c payload **itself!**
 *
 * \ingroup stringifier
 */
const char* integerValueStringify(int* payload);

/**
 * Decide which integer is less than the other
 *
 * the variable is stored in the pointer \c payload **itself!**
 *
 * \ingroup orderer
 */
int integerValueOrderer(const int* a, const int* b);

/**
 * The default orderer. Compare pointer values
 *
 * \ingroup orderer
 */
int defaultOrderer(const void* a, const void* b);

/**
 * Copy the content pointed on payload
 *
 * \ingroup cloner
 */
void* defaultPayloadCloner(void* payload);

/**
 * @return always black
 *
 * \ingroup colorer
 */
int defaultPayloadColor(void* payload);

/**
 * A possible implementation of a ::destructor. Doe nothing
 *
 * \note
 * the function does nothing
 *
 * @param[in] paylaod the payload to free
 *
 * \ingroup destructor
 */
void payloadDestructorNOP(void* payload);

/**
 * A naive implementation of a ::buffer_string. Prints the pointer of the payload
 *
 * \ingroup buffer_string
 */
int defaultBufferString(void* payload, char* buffer);

int getBufferStringOfInteger(int i, char* buffer);

//********************************* SERIALIZERS **********************************

/**
 * The default serializer. It writes the pointer itself
 *
 * \ingroup serializer
 */
int defaultSerializer(FILE* f, void* payload);

/**
 * Store inside the file the given integer
 *
 * \note
 * We won't save the bytes containing an int, so cross platforming may not work
 *
 * \ingroup serializer
 */
int serializerInt(FILE* f, int n);

// ******************************* DESERIALIZERS *************************************

/**
 * The default deserializer. It reads the pointer itself
 *
 * \ingroup deserializer
 */
int defaultDeserializer(FILE* f, void** payload);

/**
 * Load an int from the file
 *
 * \note
 * If your platform use a different number of bytes to allocate an int, this simple deserializer
 * won't work
 *
 * \ingroup deserializer
 */
int deserializerInt(FILE* f, int* n);

// ******************************* EVALUATORS *************************************

/**
 * The default evaluator. It reads the pointer itself and convert it to a number
 *
 * \ingroup evaluator
 */
int defaultEvaluator(void* p);

// ******************************* HASH FUNCTIONS **********************************

unsigned long hashString(const char* str);


#endif /* DEFAULTFUNCTIONS_H_ */
