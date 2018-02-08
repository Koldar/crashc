/*
 * csvProducer.c
 *
 *  Created on: Dec 26, 2016
 *      Author: koldar
 */


#include "csvProducer.h"
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "utility.h"
#include "log.h"

static int getSymbolTimesInString(const char* str, char symbol);
static void replaceSpacesWithDelimiterInTemplate(char* template, char delimiter);
static bool hasCSVExtension(const char* f);
static char** cloneHeader(int headerSize, char* header[]);
static void destroyHeader(int headerSize, char* header[]);

CSVHelper* initCSVHelper(const char* filePath, char delimiter, char carriageReturn, const char* template, const char** header, const char* openFormat) {
	CSVHelper* retVal = (CSVHelper*) malloc(sizeof(CSVHelper));
	if (retVal == NULL) {
		ERROR_MALLOC();
	}

	char* buffer = NULL;
	if (hasCSVExtension(filePath)) {
		buffer = strdup(filePath);
	} else {
		buffer = malloc(strlen(filePath)+4+1);
		sprintf(buffer, "%s.csv", filePath);
	}


	retVal->alreadyPrintedHeader = false;
	retVal->carriageReturn = carriageReturn;
	retVal->delimiter = delimiter;
	retVal->filePath = buffer;
	retVal->headerSize = getSymbolTimesInString(template, ' ') + 1; //given "%s %s %d" there are 3 column but only 2 spaces (" "). Hence we need to add 1
	retVal->headerNames = cloneHeader(retVal->headerSize, header);
	retVal->csvFile = fopen(buffer, openFormat);
	retVal->nextColumnIndex = 0;

	retVal->actualTemplate = strdup(template);
	replaceSpacesWithDelimiterInTemplate(retVal->actualTemplate, retVal->delimiter);
	debug("after memcpy. actualTemplate=%s", retVal->actualTemplate);

	return retVal;
}

/**
 * Free the memory from the helper
 *
 * \attention
 * The function won't free any string inside the helper. So if you,
 * when you called ::initCSVHelper, have put any non constant string (like an allocated template),
 * a <b>memory leak will appear</b>. The function just autoclose the pointer to the file and free the memory of
 * the ::CSVHelper structure
 *
 * @param[in] csvHelper the structure to remove from the memory
 */
void destroyCSVHelper(CSVHelper* csvHelper) {
	if (csvHelper != NULL) {
		fclose(csvHelper->csvFile);
	}
	destroyHeader(csvHelper->headerSize, csvHelper->headerNames);
	free(csvHelper->filePath);
	free(csvHelper->actualTemplate);
	free(csvHelper);
}

/**
 * Destroy the  header previously malloc with ::cloneHeader
 *
 * @param[in] headerSize the number of elements inside the cloned header
 * @param[in] header the header to dispose
 */
static void destroyHeader(int headerSize, char* header[]) {
	for (int i=0; i<headerSize; i++) {
		free(header[i]);
	}
	free(header);
}

/**
 * Clone the given header
 *
 * @param[in] headerSize the number of elements inside the original header
 * @param[in] header the header to clone
 * @return the clone element
 */
static char** cloneHeader(int headerSize, char* header[]) {
	char** headerClone = malloc(sizeof(char*) * headerSize);
	if (header == NULL) {
		ERROR_MALLOC();
	}

	for (int i=0; i<headerSize; i++) {
		headerClone[i] = strdup(header[i]);
	}

	return headerClone;
}

/**
 * Prints a data row inside the csv
 *
 * @param[in] csvHelper the structure that allows you to easily print data on the csv
 * @param[in] vararg the data to print.. It is of the same size of the header
 */

void tryPrintHeader(CSVHelper* csvHelper) {
	if (!csvHelper->alreadyPrintedHeader) {
		//sep options
		fprintf(csvHelper->csvFile, "sep=%c%c", csvHelper->delimiter, csvHelper->carriageReturn);
		//header print
		for (int i=0; i<csvHelper->headerSize; i++) {
			fprintf(csvHelper->csvFile, "%s", csvHelper->headerNames[i]);
			if ((i+1) < csvHelper->headerSize) {
				fprintf(csvHelper->csvFile, "%c", csvHelper->delimiter);
			}
		}
		fprintf(csvHelper->csvFile, "%c", csvHelper->carriageReturn);
		csvHelper->alreadyPrintedHeader = true;
	}
}

/**
 * Replace all the spaces inside the \c template parameter with \c delimiter characters
 *
 * \attention
 * after this operation, \c template string is changed
 *
 * @param[inout] template the template formatter used to to format a row
 * @param[in] the delimiter to use
 */
static void replaceSpacesWithDelimiterInTemplate(char* template, char delimiter) {
	char* tmp = template;
	while(*tmp != '\0') {
		if (*tmp == ' ') {
			*tmp = delimiter;
		}
		tmp++;
	}
}

/**
 * get the n-th format specifier inside \c template
 *
 * @param[in] helper the helper involved
 * @param[in] n the index of the format specfier we wantt o fetch. Indexes start from 0
 * @param[out] a buffer that will contain the nth format specifier needed
 */
void computeNColumnSpecifier(const CSVHelper* helper, int n, char* buffer) {
	char* tmp = helper->actualTemplate;
	char* tmp2 = NULL;
	int start = 0;
	int end = 0;
	while(*tmp != '\0') {
		if (n == 0) {
			tmp2 = tmp;
			while ((*tmp2 != '\0') && (*tmp2 != helper->delimiter)) {
				end++;
				tmp2++;
			}
			finest("template=%s n=%d start=%d end=%d", helper->actualTemplate, n, start, end);
			memcpy(buffer, helper->actualTemplate+start, end);
			buffer[end] = '\0';
			return;
		}
		if (*tmp == helper->delimiter) {
			//found a delimiter: decrementing number of delimiter we need to found before returning specifier
			n--;
		}
		start++;
		tmp++;
	}
}

/**
 * @param[in] str the string to analyze
 * @param[in] symbol the symbol to count
 * @return the times a particular symbol is present inside the given string \c str
 */
static int getSymbolTimesInString(const char* str, char symbol) {
	int retVal = 0;
	while (*str != '\0') {
		if (*str == symbol) {
			retVal++;
		}
		str++;
	}
	return retVal;
}

static bool hasCSVExtension(const char* f) {
	int l = strlen(f);
	return (f[l-4] == '.') && (f[l-3] == 'c') && (f[l-2] == 's') && (f[l-1] == 'v');
}

