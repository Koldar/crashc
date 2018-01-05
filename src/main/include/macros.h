/**
 * @file
 *
 * Module containing a set of general purpose macros used throughout all the project
 *
 * @author koldar
 * @date Jun 29, 2017
 */

#ifndef MACROS_H_
#define MACROS_H_

/**
 * the character used to divide tags inside a single string
 *
 * Setting this macro to '#' means that the tags are required to be defined as "hello#world".
 * See @ref tags for further information
 */
#ifndef CC_TAGS_SEPARATOR
#	define CC_TAGS_SEPARATOR ' '
#endif

/**
 * macro used to perform macro concatenation.
 *
 * @note
 * do not used this directly. Use ::TOKENPASTE2 instead
 *
 * @param[in] x the first identifier to concat
 * @param[in] y the second identifier to concat
 * @return \c x concatenated with \c y
 * @see TOKENPASTE2
 */
#define TOKENPASTE(x, y) x ## y
/**
 * Macro used to perform concatenation.
 *
 * @note
 * this is the macro you want to use to perform concatenation
 *
 * @param[in] x the first identifier to concat
 * @param[in] y the second identifier to concat
 * @return \c x concatenated with \c y
 */
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)

/**
 * Macro used to paste to a string \c v another sting, making \c v unique.
 *
 * This macro is incredible useful to produce variables names which you need to ensure are unique.
 * For example they are useful in for loops where you don't actually care about the names of the counter, but you only
 * want to fetch some data from a collection. For example:
 *
 * @code
 * int UV(a); //produce a12 assuming the code is on line 12
 * int UV(a); //produce a13 assuming the code is on line 13
 * //produce 2 different "a" variables
 * @endcode
 *
 * @param[in] v code to prepend to the line number
 * @return if \c v is a valid C identifier, this may represent a new unique variable name
 */
#define UV(v) TOKENPASTE2(v, __LINE__)


#endif /* MACROS_H_ */
