/*
 * reportProducer.c
 *
 *  Created on: Feb 16, 2017
 *      Author: koldar
 */

#include "reportProducer.h"

void reportProducerConsole(Section* sectionToDraw, Section* root, FILE* f) {
	if (sectionToDraw != root) {

	}

	reportProducerConsole(sectionToDraw->firstChild, root, f);

//	Section* el;
//	FL_SAFE_ITER(&sectionToDraw->nextSibling, el) {
//
//	}
}


