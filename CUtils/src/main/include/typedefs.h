/*
 * typedefs.h
 *
 *  Created on: May 17, 2017
 *      Author: koldar
 */

#ifndef TYPEDEFS_H_
#define TYPEDEFS_H_

#include <stdio.h>
#include <stdbool.h>

/**
 * Represents a pair of integers
 */
typedef struct int_pair {
	///the first integer
	int x;
	///the second integer
	int y;
} int_pair;

/**
 * Represents the command we need to execute when drawing the graph with ::printGraphWithOptions
 */
typedef enum {
	/**
	 * We will use "dot" command, useful for standard direct graphs
	 */
	DC_DOT,
	/**
	 * We will use "circo" command, useful for circular graphs
	 */
	DC_CIRCO,
	/**
	 * We will use "neato" command, useful for undirect graphs
	 */
	DC_NEATO,
	/**
	 * We will use "sfdp" command, useful to large undirect graphs
	 */
	DC_SFDP
} dot_commands;

typedef bool (*finder)(void*);
typedef void (*destructor)(void*);
typedef unsigned long (*hashFunction)(void*);
typedef void* (*cloner)(void*);
typedef bool (*comparator)(void*, void*);
typedef const char* (*stringify)(void*);
/**
 * Represents anything that complies with buffer string.
 *
 * A buffer string function is something that accepts the thing whose string representation you
 * want to obtain and a buffer that will store such string representation.
 *
 * The return value represents the number of bytes consumed by the first parameter inside the buffer
 *
 * \note
 * This function is heavily **not** thread-safe
 */
typedef int (*buffer_string)(void*,char*);

/**
 * Given an object, the function will compute its color
 *
 * The function is useful while debugging
 */
typedef int (*colorer)(void*);

/**
 * Given 2 object, determine if the first is less, equal of greater than the second one
 *
 * The function returns:
 * \li a negative number if \f$ a < b \f$;
 * \li 0 if  \f$ a = b\f$;
 * \li a positive number if \f$ a > b \f$;
 */
typedef int (*orderer)(void*, void*);

/**
 * Given a stream of char, parse the first characters read from the stream into an object.
 *
 * The first function parameter is the stream to read from; the second parameter is a pointer that we
 * will alter: after the call, it will contained the parsed data; the return value represents the number of
 * characters read from the stream.
 *
 */
typedef int (*object_deserializer)(FILE*, void**);

typedef int (*object_serializer)(FILE*, void*);


#endif /* TYPEDEFS_H_ */
