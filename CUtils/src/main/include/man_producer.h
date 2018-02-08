/*
 * man_producer.h
 *
 *  Created on: Aug 25, 2017
 *      Author: koldar
 */

#ifndef MAN_PRODUCER_H_
#define MAN_PRODUCER_H_

#include <arg_parser.h>

typedef struct man_producer man_producer;

man_producer* initManProducer();

void destroyManProducer(man_producer* mp);

void setApplicationNameOfManProducer(man_producer* mp, const char* name);

void setBriefDescriptionOfManProducer(man_producer* mp, const char* name);

void setLongDescriptionOfManProducer(man_producer* mp, const char* name);

/**
 * @param[in] ... each parameter in the variadic is a string. An example of string might be <tt>basename(3)</tt>
 */
void addSeeAlsoOfManProducer(man_producer* mp, int count, ...);

/**
 * @param[in] ... each parameter in the variadic is a string. An example of such string might be <tt>input file needs to exist, otherwise the program crashes</tt>
 */
void addBugsOfManProducer(man_producer* mp, int count, ...);

void addAuthorOfManProducer(man_producer* mp, const char* fullName, const char* email);

void setSynopsysOfManProducer(man_producer* mp, const char* synopsys);

/**
 * @param[in] ... each parameter in the variadic is a string. An example
 */
void addOptionsOfManProducer(man_producer* mp, ...);

void populateManProducerFromCLIParser(man_producer* mp, const CLIParser* p, bool overwriteCurrentOptions);

void populateManProducerFromGroffFile(man_producer* mp, const char* groffFileName, bool overwriteCurrentOptions);

void produceGroffFile(const man_producer* mp);

#endif /* MAN_PRODUCER_H_ */
