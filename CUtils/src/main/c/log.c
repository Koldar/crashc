/*
 * log.c
 *
 *  Created on: Dec 25, 2016
 *      Author: koldar
 */

#include "log.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <libgen.h>
#include <stdlib.h>

const char* LOGLEVEL_TO_STRING[LOG_LEVEL_SIZE] = {
		"ALL",
		"DEBUG",
		"FINEST",
		"FINER",
		"FINE",
		"INFO",
		"WARNING",
		"ERROR",
		"CRITICAL",
		"NOLOG"
};

typedef struct Names {
	char* name;
	//the hash of the name
	unsigned long hashName;
	struct Names* next;
} Names;

/**
 * The level our log is operating on
 */
static LogLevel logLevel = LOG_ALL;
/**
 * List of loggers (aka files) that we won't track
 */
static Names* excludedLoggers = NULL;
/**
 * The default buffer used by log module to emit logs into  log destination
 */
char __default_log_buffer[LOG_BUFFER_SIZE];

static void logToStdOut(const char* fileName, const char* functionName, const int lineNo, const LogLevel level, const char* message);
static void logToDevNull(const char* fileName, const char* functionName, const int lineNo, const LogLevel level, const char* message);
static void addHeadInNames(Names** names, const char* name);
static void removeFromNames(Names** names, const char* name);
static void removeAllNames(Names** names);
static bool isExcluded(Names** names, const char* name);
static unsigned long hashDjb2(const char *str);

/**
 * A function we will use to log the message to the physical storage (either a database, the network or a file)
 */
static appenderFunction appenders = &logToStdOut;

void setLevel(LogLevel level) {
	logLevel = level;
}

/**
 * Avoid to print everything coming from a particular logger
 *
 * @param[in] logger the logger that from this time until ::includeLogger call won't be tracked down at all
 */
void excludeLogger(const char* logger) {
	addHeadInNames(&excludedLoggers, logger);
}

/**
 * Include in the logging the logs coming from this logger
 *
 * @param[in] logger the logger to include in the log output
 */
void includeLogger(const char* logger) {
	removeFromNames(&excludedLoggers, logger);
}

void clearExcludedLoggers() {
	removeAllNames(&excludedLoggers);
}

void clearAppenders() {
	appenders = &logToDevNull;
}

void __generic_log_function(const LogLevel level, const char* absoluteFilePath, const char* functionName, const int lineNo, int logBufferSize, char* logBuffer, const char* format, ...) {
	if (level < logLevel) {
		return;
	}
	char* fileName = basename(absoluteFilePath);
	if (isExcluded(&excludedLoggers, fileName)) {
		return;
	}
	va_list argList;
	va_start(argList, format);
	vsnprintf(logBuffer, logBufferSize, format, argList);
	va_end(argList);
	appenders(fileName, functionName, lineNo, level, logBuffer);

}


static void addHeadInNames(Names** names, const char* name) {
	Names* retVal = malloc(sizeof(Names));
	if (retVal == NULL) {
		return;
	}
	char* nameCopy = malloc(strlen(name)+1);
	if (nameCopy == NULL) {
		return;
	}
	strcpy(nameCopy, name);
	retVal->name = nameCopy;
	retVal->hashName = hashDjb2(retVal->name);
	retVal->next = NULL;

	if (*names != NULL) {
		retVal->next = (*names);
	}
	(*names) = retVal;
}

static void removeAllNames(Names** names) {
	Names* tmp = *names;
	Names* tmp2 = NULL;
	while(tmp != NULL) {
		tmp2 = tmp->next;
		free(tmp->name);
		free(tmp);
		tmp = tmp2;
	}
	*names = NULL;
}

static void removeFromNames(Names** names, const char* name) {
	Names* tmp = *names;
	Names* old = NULL;
	unsigned long hashName = hashDjb2(name);

	while (tmp != NULL) {
		if ( tmp->hashName == hashName) {
			if (old == NULL) {//we need to remove the head
				(*names) = tmp->next;
			} else {
				old->next = tmp->next;
				free(tmp->name);
				free(tmp);
			}

		}
		old = tmp;
		tmp = tmp->next;
	}
}

static bool isExcluded(Names** names, const char* name) {
	if (*names == NULL) {
		return false;
	}
	Names* tmp = *names;
	unsigned long hashName = hashDjb2(name);
	while (tmp != NULL) {
		if (tmp->hashName == hashName) {
			return true;
		}
		tmp = tmp->next;
	}
	return false;
}

/**
 * hash function using djb2 algorithm
 *
 * For further information please see <a href="http://www.cse.yorku.ca/~oz/hash.html">hash algorithms</a>
 *
 * @param[in] str the string to hash
 * @return the hash value
 */
static unsigned long hashDjb2(const char *str) {
	unsigned long hash = 5381;
	int c;
	while (c = *str++)
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	return hash;
}

static void logToStdOut(const char* fileName, const char* functionName, const int lineNo, const LogLevel level, const char* message) {
	printf("%s:%s:%d[%s] %s\n", fileName, functionName, lineNo, LOGLEVEL_TO_STRING[level], message);
	fflush(stdout);
}

static void logToDevNull(const char* fileName, const char* functionName, const int lineNo, const LogLevel level, const char* message) {

}

