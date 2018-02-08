/*
 * macrosTest.c
 *
 *  Created on: Jun 27, 2017
 *      Author: koldar
 */

#include <assert.h>
#include "CuTest.h"
#include "macros.h"

void testINITALIZE_VARS01(CuTest* tc) {

}

void testINITALIZE_VARS02(CuTest* tc) {

}

CuSuite* CuMacroSuite() {
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, testINITALIZE_VARS01);

	return suite;
}
