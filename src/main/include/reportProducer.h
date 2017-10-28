/*
 * reportProducer.h
 *
 *  Created on: Feb 15, 2017
 *      Author: koldar
 */

#ifndef REPORTPRODUCER_H_
#define REPORTPRODUCER_H_

#ifndef ALL_IN_ONE
	#include <stdio.h>
	#include <stdlib.h>
#endif

#include "section.h"


/**
 * Represents which implementation we need to use in ::produceReport
 */
typedef struct ReportProducerImplementation {
	void (*generateHeader)(Section* root);
	void (*generateFooter)(Section* root);
	void (*handleSectionStarted)(Section* s, int depth);
	void (*handleSectionFinished)(Section* s, int depth);
	void (*handleAssertionStarted)(TestReport* tr,int depth);
	void (*handleAssertion)(TestReport* tr, int depth);
	void (*handleAssertionFinished)(TestReport* tr, int depth);
	void (*handleLastAssertionAgain)(TestReport* tr, int depth);
} ReportProducerImplementation;

extern ReportProducerImplementation consoleProducer;

void produceReport(ReportProducerImplementation rpi, Section* sectionTree);


#endif /* REPORTPRODUCER_H_ */
