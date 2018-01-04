/**
 * @file
 *
 * Module collecting all the error macros used in the project
 *
 * You can compile time customize the code when a specific error occurs.
 * Normally it simply call \c exit function with a specific error code
 *
 * @author koldar
 * @date Feb 15, 2017
 */

#ifndef ERRORS_H_
#define ERRORS_H_

/**
 * C code to execute when a \c malloc returns NULL
 *
 */
#ifndef MALLOCERRORCALLBACK
#	define MALLOCERRORCALLBACK() exit(1)
#endif



#endif /* ERRORS_H_ */
