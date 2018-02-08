/*
 * \file conversions.h
 *
 * A module containing all the function used to convert from and to basic C data.
 * Think of this module as a easy frontend to "strtoX" functions available in C.
 *
 *
 *  Created on: Sep 15, 2017
 *      Author: koldar
 */

#ifndef CONVERSIONS_H_
#define CONVERSIONS_H_

#include <stdbool.h>

/**
 * Convert a string to a double
 *
 * @param[in] str the string to convert
 * @param[out] output if the function returns \c true, the pointer is populated with the converted value
 * @return
 * 	\li true if we were able to convert the string;
 * 	\li false otherwise
 */
bool convertString2Double(char* str, double* output);

/**
 * Convert a string to a int
 *
 * @param[in] str the string to convert
 * @param[out] output if the function returns \c true, the pointer is populated with the converted value
 * @return
 * 	\li true if we were able to convert the string;
 * 	\li false otherwise
 */
bool convertString2Int(char* str, int* output);

/**
 * Convert a string to a long
 *
 * @param[in] str the string to convert
 * @param[out] output if the function returns \c true, the pointer is populated with the converted value
 * @return
 * 	\li true if we were able to convert the string;
 * 	\li false otherwise
 */
bool convertString2Long(char* str, long* output);

/**
 * Convert a string to a boolean
 *
 * A string is considered to be "true" if is equal (case insensitive) to either one of those:
 * \li true;
 * \li yes;
 * \li ok;
 * \li 1;
 *
 * A string is considered to be "false" if is equal (case insensitive) to either one of those:
 * \li false;
 * \li no;
 * \li ko;
 * \li 0;
 *
 * @param[in] str the string to convert
 * @param[out] output if the function returns \c true, the pointer is populated with the converted value
 * @return
 * 	\li true if we were able to convert the string;
 * 	\li false otherwise
 */
bool convertString2Bool(char* str, bool* output);


#endif /* CONVERSIONS_H_ */
