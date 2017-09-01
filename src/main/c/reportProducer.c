/*
 * reportProducer.c
 *
 *  Created on: Feb 16, 2017
 *      Author: koldar
 */

#include "reportProducer.h"
#include "console_color.h"
#include <stdarg.h>


void fcprintf(FILE* f, ConsoleColor c, const char* template, ...);
static void reportProducerConsoleRecursive(Section* sectionToDraw, int depth, Section* root, FILE* f);
static void reportProducerRecursive(ReportProducerImplementation rpi, Section* sectionToDraw, int depth, Section* root);

static void generateConsoleHeader(Section* root);
static void generateConsoleFooter(Section* root);
static void handleConsoleSectionStarted(Section* s, int depth);
static void handleConsoleAssertionStarted(TestReport* tr,int depth);
static void handleConsoleAssertion(TestReport* tr, int depth);
static void handleConsoleAssertionFinished(TestReport* tr, int depth);
static void handleConsoleLastAssertionAgain(TestReport* tr, int depth);
static void handleConsoleSectionFinished(Section* s, int depth);

ReportProducerImplementation consoleProducer = {
		generateConsoleHeader,
		generateConsoleFooter,
		handleConsoleSectionStarted,
		handleConsoleSectionFinished,
		handleConsoleAssertionStarted,
		handleConsoleAssertion,
		handleConsoleAssertionFinished,
		handleConsoleLastAssertionAgain
};

void produceReport(ReportProducerImplementation rpi, Section* sectionTree) {
	rpi.generateHeader(sectionTree);
	reportProducerRecursive(rpi, sectionTree, 0, sectionTree);
	rpi.generateFooter(sectionTree);
}

static void reportProducerRecursive(ReportProducerImplementation rpi, Section* sectionToDraw, int depth, Section* root) {
	if (sectionToDraw == NULL) {
		return;
	}
	if (sectionToDraw != root) {
		//we print on the console only if the section involved is not the root (the root is a dummy section)
		int i;

		rpi.handleSectionStarted(sectionToDraw, depth);
		ITERATE_ON_LIST(sectionToDraw->assertionReportList, cell, el, TestReport) {
			rpi.handleAssertionStarted(el, depth);
			rpi.handleAssertion(el, depth);
			rpi.handleAssertionFinished(el, depth);
		}
		rpi.handleLastAssertionAgain(el, depth);
		rpi.handleSectionFinished(sectionToDraw, depth);
	}

	Section* el;
	reportProducerRecursive(rpi, sectionToDraw->firstChild, depth+1, root);
	el = sectionToDraw->nextSibling;
	while (el != NULL) {
		reportProducerRecursive(rpi, el, depth, root);
		el = el->nextSibling;
	}
}

static void generateConsoleHeader(Section* root) {
	fcprintf(stdout, ANSI_COLOR_BLUE, "***************** TEST REPORT *****************\n");
}

static void generateConsoleFooter(Section* root) {

}

static void handleConsoleSectionStarted(Section* s, int depth) {
	for (int i=0; i<(depth-1); i++) {
		fprintf(stdout, " ");
	}
	fcprintf(
			stdout,
			getLengthOfList(s->failureReportList) == 0 ? ANSI_COLOR_GREEN: ANSI_COLOR_RED,
					"%s:\n",
					s->description
	);
}

static void handleConsoleAssertionStarted(TestReport* tr,int depth) {

}

static void handleConsoleAssertion(TestReport* tr, int depth) {
	if (!getOutcomeFromTestReport(tr)) {
		fcprintf(stdout,
				ANSI_COLOR_RED,
				"%s[%d]: %s\n",
				getFileTestedOfTestReport(tr),
				getLineNoOfTestReport(tr),
				getExprTestedOfTestReport(tr)
		);
	}
}

static void handleConsoleAssertionFinished(TestReport* tr, int depth) {

}

static void handleConsoleLastAssertionAgain(TestReport* tr, int depth) {

}

static void handleConsoleSectionFinished(Section* s, int depth) {

}

void reportProducerConsole(Section* sectionToDraw, FILE* f) {
	reportProducerConsoleRecursive(sectionToDraw, 0, sectionToDraw, f);
}

static void reportProducerConsoleRecursive(Section* sectionToDraw, int depth, Section* root, FILE* f) {
	if (sectionToDraw == NULL) {
		return;
	}
	if (sectionToDraw != root) {
		//we print on the console only if the section involved is not the root (the root is a dummy section
		int i;


		for (int i=0; i<depth; i++) {
			fprintf(stdout, " ");
		}

		fcprintf(
				stdout,
				getLengthOfList(sectionToDraw->failureReportList) == 0 ? ANSI_COLOR_GREEN: ANSI_COLOR_RED,
						"%s:\n",
						sectionToDraw->description
		);
		ITERATE_ON_LIST(sectionToDraw->failureReportList, cell, el, TestReport) {
			//TODO customize colors
			fcprintf(stdout,
					ANSI_COLOR_RED
					"file=%s, lineno=%d, assert=%s\n",
					getFileTestedOfTestReport(el),
					getLineNoOfTestReport(el),
					getExprTestedOfTestReport(el)
			);
		}
	}

	Section* el;
	reportProducerConsoleRecursive(sectionToDraw->firstChild, depth+1, root, f);
	el = sectionToDraw->nextSibling;
	while (el != NULL) {
		reportProducerConsoleRecursive(el, depth, root, f);
		el = el->nextSibling;
	}
}

/**
 * Prints a string in a file using the color c
 *
 */
void fcprintf(FILE* f, ConsoleColor c, const char* template, ...) {
	va_list va;
	va_start(va, template);
	fprintf(f, c);
	vfprintf(f, template, va);
	fprintf(f, ANSI_COLOR_RESET);
	va_end(va);
}




