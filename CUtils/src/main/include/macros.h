/*
 * macros.h
 *
 *  Created on: May 17, 2017
 *      Author: koldar
 */

#ifndef MACROS_H_
#define MACROS_H_

#include <stdio.h>
#include <stdlib.h>

#define VALUE(x) x
#define XSTR(x) #x
#define STR(x) XSTR(x)
#define CONCAT(x,y) x##y

#define TOKENPASTE(x, y) x ## y

#define TOKENPASTE2(x, y) TOKENPASTE(x, y)

/**
 * Swap the contents of 2 variables
 *
 * An example of usage:
 *
 * @code
 * 	int* a = 0x555555;
 * 	int* b = 0x666666;
 * 	SWAP(a, b, int*);
 * @endcode
 *
 * @param[in] x the first variable
 * @param[in] y the second variable
 * @param[in] type the type of the variable \c x and \c y
 */
#define SWAP(x, y, type) { \
	type UV(tmp) = (x); \
	(x) = (y); \
	(y) = UV(tmp); \
}

/**
 * Unique Variable
 *
 * Generate a unique name of a particular varaible. Useful in macro definitions
 *
 * See <a="http://stackoverflow.com/a/1597129/1887602">here</a> for further information
 */
#define UV(v) TOKENPASTE2(v, __LINE__)

/**
 * \def STATICR
 *
 * This macro is replaced with "static" reserved word during release compilation.
 * If DEBUG macro is present, the macro is not replaced with anything.
 *
 * This macro is useful during debugging: you might want to test some internal component of your project.
 * Use this macro instead of the simple "static" reserved word: in this way during debugging you will be able to see it whilst
 * in production the function tested will be hidden
 */
#ifndef DEBUG
#	define STATICR static
#else
#	define STATICR
#endif

/**
 * \def EXPORT_SYMBOL
 *
 * This macro will allow you to add specific code to alter the linker exporting policy during building.
 * Annotate every function or variable you want to export in your shared library with EXPORT_SYMBOL.
 * Everything not annotated will, by default, not be exported at all. In this way you can avoid publishing stuff you don't want to export at all.
 *
 * During the compilation of your library please set the definition "BUILDING_LIBRARY" (ie via "-DBUILDING_LIBRARY").
 * When using the library, do your usual stuff.
 */
#if BUILDING_LIBRARY
#define EXPORT_SYMBOL __attribute__((__visibility__("default")))
#else
#define EXPORT_SYMBOL
#endif

#if BUILDING_LIBRARY
#define DONT_EXPORT_SYMBOL
#else
#define DONT_EXPORT_SYMBOL
#endif

#define ERRORMSG(code, msg, ...) \
	fprintf(stderr, "%s:%s[%d] ", __FILE__, __func__, __LINE__);\
	fprintf(stderr, msg, ## __VA_ARGS__);\
	fprintf(stderr, "\n");\
	exit(code)

/**
 * Error to use when a malloc return NULL
 */
#define ERROR_MALLOC() ERRORMSG(1, "Malloc failure!")

/**
 * Error to use when fopen fails
 */
#define ERROR_FILE(file) ERRORMSG(2, "Bad permission while handling file \"%s\"", file)

/**
 * Error to use when you failed to add an element to something else because such something else is actually full.
 *
 * I usually use it on array, stacks and so on
 */
#define ERROR_OBJECT_IS_FULL(object, size) ERRORMSG(3, "Object \"%s\" has reached its maximum capacity %d!", object, size)

#define ERROR_NOT_SUCH_OBJECT(objectDsc, objectId) ERRORMSG(4, "Object %s %d has not been found!", objectDsc, objectId);

#define ERROR_CONSTRUCTION_FAILED(objectDsc, objectId) ERRORMSG(5, "%s cannot be created by using \"%s\"!", objectDsc, objectId)

/**
 * Error to use when you have tried (but failed) to apply something to something else.
 *
 * I usually use it for regex on string
 */
#define ERROR_APPLICATION_FAILED(applierDsc, applier, appliedDsc, applied) ERRORMSG(6, "%s \"%s\" cannot be applied to %s \"%s\"!", applierDsc, applier, appliedDsc, applied)

#define ERROR_EQUALITY_FAILED(object1Dsc, object1, object2Dsc, object2) ERRORMSG(7, "%s \"%s\" isn't the same of %s \"%s\"!", object1Dsc, object1, object2Dsc, object2)

/**
 * Error to show when the code arrives to a case scenario that isn't supported
 *
 * I tend to exploit this in switch cases
 */
#define ERROR_UNHANDLED_CASE(scenarioDsc, scenario) ERRORMSG(8, "Case \"%d\" within \"%s\" not handled at all", scenario, scenarioDsc)

/**
 * Error to show when the code goes into a not implemented feature
 */
#define ERROR_NOT_IMPLEMENTED() ERRORMSG(9, "Not implemented yet error!")

#define ERROR_NULL(dsc) ERRORMSG(10, "%s is unexpectly NULL!", dsc)

#define ERROR_IS_ALREADY_PRESENT(object, container, format) \
	ERRORMSG(11, "object \"" format "\" is already inside \"%s\"!", object, container)

/**
 * Error to show when you need to add an error but you know the program shouild neve go there
 */
#define ERROR_IMPOSSIBLE_SCENARIO(scenarioDsc, ...) \
	ERRORMSG(12, "The software entered in a scenario that shouldn't happen at all! There is an high chance this is a BUG! Here's a description:\n" scenarioDsc, ## __VA_ARGS__)

#ifndef BUFFER_SIZE
#	define BUFFER_SIZE 300
#endif

/**
 * @def LONG_BUFFER_SIZE
 *
 * Represents a size of a buffer much longer than ::BUFFER_SIZE. It usually
 * is 5 times bigger than ::BUFFER_SIZE
 */
#ifndef LONG_BUFFER_SIZE
#	define LONG_BUFFER_SIZE (5*BUFFER_SIZE)
#endif

/**
 * CUtils contains several static array used to store temporary information.
 * If the size of such arrays is too small for your application, you can increase their size by teaking with this macro
 */
#ifndef CUTILS_ARRAY_SIZE
#	define CUTILS_ARRAY_SIZE 1000
#endif


#endif /* MACROS_H_ */
