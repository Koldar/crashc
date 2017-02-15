/*
 * assertions.h
 *
 *  Created on: Jan 13, 2017
 *      Author: koldar
 */

#ifndef ASSERTIONS_H_
#define ASSERTIONS_H_

#include "crashC.h"

/**
 * Executed when an assertion fails
 *
 * @param[in] section the section we're in right now
 * @param[in] expr a string representing the code written by the developer
 * @param[in] file a string containing the file we're in right now
 * @param[in] line the line in the source code we're in right now
 */
void __assert_fail(Section* section, const char* expr, const char* file, const int line);


#define ASSERT(expr) expr ? 0 : __assert_fail(currentSection, #expr, __FILE__, __LINE__)
#define assert(expr) ASSERT(expr)

#endif /* ASSERTIONS_H_ */
