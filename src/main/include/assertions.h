/*
 * assertions.h
 *
 *  Created on: Jan 13, 2017
 *      Author: koldar
 */

#ifndef ASSERTIONS_H_
#define ASSERTIONS_H_

#include "oldcrashC.h"

/**
 * Trace the assertion inside the test framework report infrastructure
 *
 * @param[in] section the section we're in right now
 * @param[in] expr a string representing the code written by the developer
 * @param[in] file a string containing the file we're in right now
 * @param[in] line the line in the source code we're in right now
 * @param[in] outcome true if the assertion was successful, false otherwise
 */
void __assert(Section* section, const char* expr, const char* file, const int line, const bool outcome);

#define ASSERT(expr) __assert(currentSection, #expr, __FILE__, __LINE__, (expr))
#define assert(expr) ASSERT(expr)

#endif /* ASSERTIONS_H_ */
