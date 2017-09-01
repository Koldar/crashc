/*
 * assertions.c
 *
 *  Created on: Feb 8, 2017
 *      Author: koldar
 */


#include "assertions.h"
#include <stdio.h>
#include <stdlib.h>

void __assert(Section* section, const char* expr, const char* file, const int line, const bool outcome) {
	//TODO add stop at first error
	TestReport* tr = initTestReport(file, line, expr, outcome);
	addTailInList(section->assertionReportList, tr);
	if (!outcome) {
		addTailInList(section->failureReportList, tr);
		fprintf(stderr, "%s:%d the expression \"%s\" failed.\n", file, line, expr);
	}
}