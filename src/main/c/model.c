/*
 * model.c
 *
 *  Created on: Oct 11, 2017
 *      Author: koldar
 */

#include <stdlib.h>
#include "testReport.h"
#include "model.h"

crashc_model setupDefaultMainModel() {
	crashc_model retVal;

	retVal.currentSection = NULL;
	retVal.currentSnapshot = NULL;
	retVal.test_reports_list = initList();
	retVal.testCaseInvolved = NULL;
	retVal.suites_array_index = 0;
	retVal.runOnlyIfTags = initHT();
	retVal.excludeTags = initHT();
	retVal.rootSection = initSection(ST_ROOT, 0, "root", "");

	return retVal;
}

void tearDownModel(crashc_model ccm) {
	destroySection(ccm.rootSection);
	destroyHTWithElements(ccm.excludeTags, destroyTag);
	destroyHTWithElements(ccm.runOnlyIfTags, destroyTag);
	destroyListWithElement(ccm.test_reports_list, destroyTestReport);
}
