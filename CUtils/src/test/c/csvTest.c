/*
 * csvTest.c
 *
 *  Created on: May 22, 2017
 *      Author: koldar
 */

#include <assert.h>
#include "CuTest.h"
#include "log.h"
#include "csvProducer.h"


void testcsvProducer01(CuTest* tc) {
	const char* header[] = {"foo", "bar", "hello"};

	debug("initialize csvHelper...");
	CSVHelper* csvHelper = initCSVHelper(__func__, ',', '\n', "%d %5.2f %s", header, "w");
	debug("done");

	assert(csvHelper->headerSize == 3);

	debug("printing data row...");
	printDataRow(csvHelper, 5, 3.2, "string1");
	debug("done");

	printDataRow(csvHelper, 3, 3.2565, "string2");
	printDataRow(csvHelper, 2, 343.2, "string3");
	printDataRow(csvHelper, 1, 35.2, "string4");

	destroyCSVHelper(csvHelper);
}

void testcsvProducer02(CuTest* tc) {
	const char* header[] = {"foo", "bar", "hello"};

	debug("initialize csvHelper...");
	CSVHelper* csvHelper = initCSVHelper(__func__, ',', '\n', "%d %d %d", header, "w");
	debug("done");

	assert(csvHelper->headerSize == 3);

	debug("printing data row...");
	for (int i=0; i<3; i++) {
		printSingleDataInRow(csvHelper, i);
	}
	printDataRow(csvHelper, 5, 5, 5);
	printDataRow(csvHelper, 5, 5, 5);
	printDataRow(csvHelper, 5, 5, 5);

	destroyCSVHelper(csvHelper);
}

CuSuite* CuCSVSuite() {
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, testcsvProducer01);
	SUITE_ADD_TEST(suite, testcsvProducer02);

	return suite;
}
