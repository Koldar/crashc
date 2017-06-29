/*
 * macros.h
 *
 *  Created on: Jun 29, 2017
 *      Author: koldar
 */

#ifndef MACROS_H_
#define MACROS_H_


#define TOKENPASTE(x, y) x ## y
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)

/**
 * Macro used to paste to a string \c v another sting, making \c v unique.
 *
 * This macro is incredible useful to produce variables names which you need to ensure are unique.
 * For example they are useful in for loops where you don't actually care about the names of the counter, but you only
 * want to fetch some data from a collection. For example:
 *
 * int UV(a);
 * int UV(a);
 * //produce 2 different "a" variables
 *
 */
#define UV(v) TOKENPASTE2(v, __LINE__)

#define PUBLIC
#define PRIVATE


#endif /* MACROS_H_ */
