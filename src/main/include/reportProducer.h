/*
 * reportProducer.h
 *
 *  Created on: Feb 15, 2017
 *      Author: koldar
 */

#ifndef REPORTPRODUCER_H_
#define REPORTPRODUCER_H_

#include "section.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Represents which implementation we need to use in ::produceReport
 */
typedef enum ReportImplementation {
	///the report is put inside a file
	RI_FILE,
	///the report is put inside a XML
	RI_XML
} ReportImplementation;


void produceReport(ReportImplementation ri, Section* sectionTree);


void reportProducerConsole(Section* sectionToDraw, Section* root, FILE* f);


#endif /* REPORTPRODUCER_H_ */
