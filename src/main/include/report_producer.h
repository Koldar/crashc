/*
 * report_producer.h
 *
 *  Created on: Oct 27, 2017
 *      Author: Lorenzo Nodari <lorenzo.nodari.it@ieee.org>
 */

#ifndef REPORT_PRODUCER_H_
#define REPORT_PRODUCER_H_

#include "section.h"
#include "testReport.h"
#include <stdio.h>

/**
 * Prints the string representation of a snapshot status
 */
char * ct_snapshot_status_to_string(snapshot_status status);

/**
 * Prints the string representation of a section type
 */
char * ct_section_type_to_string(section_type type);

/**
 * Prints the given test report on stdout with a default format
 */
void ct_stdout_report(TestReport * report);

/**
 * Prints the test report on a given file, with a default format
 */
void ct_fprint_report(FILE * file, TestReport * report);

/**
 * Prints a string containing default infos about a snapshot tree on the given file
 */
void ct_print_snapshot_tree(FILE * file, SectionSnapshot * snapshot, int level);

#endif /* REPORT_PRODUCER_H_ */
