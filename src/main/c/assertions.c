/*
 * assertions.c
 *
 *  Created on: Feb 8, 2017
 *      Author: koldar
 */


#include "assertions.h"
#include <stdio.h>
#include <stdlib.h>

void __assert_fail(Section* section, const char* expr, const char* file, const int line) {
	fprintf(stderr, "%s:%d the expression \"%s\" failed.", file, line, expr);
}
