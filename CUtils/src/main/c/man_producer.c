/*
 * man_producer.c
 *
 *  Created on: Aug 25, 2017
 *      Author: koldar
 */

#include <stdarg.h>
#include "man_producer.h"
#include "macros.h"
#include "list.h"


struct man_producer {
	char* name;
	char* briefDescription;
	char* description;
	char* synopsis;
	char* version;
	int sectionLevel;
	string_list* seeAlsoList;
	string_list* bugList;
	string_list* authorList;
	string_list* emailList;
};

man_producer* initManProducer() {
	man_producer* retVal = malloc(sizeof(man_producer));
	if (retVal == NULL) {
		ERROR_MALLOC();
	}

	retVal->briefDescription = NULL;
	retVal->bugList = initList();
	retVal->description = NULL;
	retVal->name = NULL;
	retVal->sectionLevel = 0;
	retVal->seeAlsoList = initList();

	return retVal;
}

void destroyManProducer(man_producer* mp) {
	destroyList(mp->bugList);
	destroyList(mp->seeAlsoList);
	free(mp);
}

void setApplicationNameOfManProducer(man_producer* mp, const char* name) {
	mp->name = name;
}

void setSectionNumberOfManProducer(man_producer* mp, int sectionLevel) {
	if (sectionLevel<1 || sectionLevel>8) {
		ERROR_APPLICATION_FAILED("section level", sectionLevel, "man producer", mp);
	}

	mp->sectionLevel = sectionLevel;
}

void setBriefDescriptionOfManProducer(man_producer* mp, const char* name) {
	mp->briefDescription = name;
}

void setLongDescriptionOfManProducer(man_producer* mp, const char* name) {
	mp->description = name;
}

void addSeeAlsoOfManProducer(man_producer* mp, int count, ...) {
	va_list va;

	va_start(va, mp);
	for (int i=0; i<count; i++) {
		char* seeAlso = va_arg(va, char*);
		addTailInList(mp->seeAlsoList, seeAlso);
	}
	va_end(va);
}

void addBugsOfManProducer(man_producer* mp, int count, ...) {
	va_list va;

	va_start(va, mp);
	for (int i=0; i<count; i++) {
		char* bug = va_arg(va, char*);
		addTailInList(mp->bugList, bug);
	}
	va_end(va);
}

void addAuthorOfManProducer(man_producer* mp, const char* fullName, const char* email) {
	addTailInList(mp->authorList, fullName);
	addTailInList(mp->emailList, email);
}

void setSynopsysOfManProducer(man_producer* mp, const char* synopsys) {
	mp->synopsis = synopsys;
}

void setVersionOfManProducer(man_producer* mp, char* version) {
	mp->version = version;
}

void setOptionsOfManProducer(man_producer* mp, ...) {

}

void populateManProducerFromCLIParser(man_producer* mp, const CLIParser* p, bool overwriteCurrentOptions) {

}

void populateManProducerFromGroffFile(man_producer* mp, const char* groffFileName, bool overwriteCurrentOptions) {

}

void produceGroffFile(const man_producer* mp) {
	char buffer[300];


	sprintf(buffer, "%s.1", mp->name);
	FILE* fout = fopen(buffer, "w");
	if (fout == NULL) {
		ERROR_FILE(buffer);
	}

	fprintf(fout, ".TH %s %d %s %s\n\n", mp->name, mp->sectionLevel, __DATE__, mp->version);

	fprintf(fout, ".SH NAME\n");
	fprintf(fout, "%s - %s\n\n", mp->name, mp->briefDescription);

	fprintf(fout, ".SH SYNOPSIS\n");
	fprintf(fout, "%s\n\n", mp->synopsis);

	fprintf(fout, ".SH DESCRIPTION\n");
	fprintf(fout, "%s\n\n", mp->description);

	fprintf(fout, ".SH OPTIONS\n");
	//fprintf

	fprintf(fout, ".SH BUGS\n");
	if (isEmptyList(mp->bugList)) {
		fprintf(fout, "No Known bugs to declare.\n");
	} else {
		int i=0;
		ITERATE_ON_LIST(mp->bugList, cell, bug, char) {
			i++;
			fprintf(fout, " %d. %s;\n", i, bug);
		}
	}
	fprintf(fout, "\n");

	fprintf(fout, ".SH AUTHOR\n");
	for (int i=0; i<getLengthOfList(mp->authorList); i++) {
		fprintf(fout, " - %s contactable with %s\n", getNthElementOfList(mp->authorList, i), getNthElementOfList(mp->emailList, i));
	}
	fprintf(fout, "\n");

	if (!isEmptyList(mp->seeAlsoList)) {
		fprintf(fout, ".SH SEE ALSO\n");
		ITERATE_ON_LIST(mp->seeAlsoList, cell, seeAlso, char) {
			fprintf(fout, " - %s\n", seeAlso);
		}
		fprintf(fout, "\n");
	}

	fclose(fout);
}
