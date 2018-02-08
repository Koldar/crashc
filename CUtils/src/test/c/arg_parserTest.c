/*
 * arg_parserTest.c
 *
 *  Created on: Aug 22, 2017
 *      Author: koldar
 */

#include "CuTest.h"
#include "arg_parser.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"

void testArgParser01(CuTest* tc) {
	int nodes;

	CLIParser* parser = initCLIParser();

	addOptionalIntOptionToCLIParser(parser, 'n', "nodes", 5, &nodes, "node");

	const char* args[] = {"Example", "-n6"};
	const int argc = 2;
	parseCLIArgumentsWithCLIParser(parser, argc, args);

	assert(nodes == 6);

	destroyCLIParser(parser);
}

void testArgParser02(CuTest* tc) {
	int nodes;

	CLIParser* parser = initCLIParser();

	addOptionalIntOptionToCLIParser(parser, 'n', "nodes", 5, &nodes, "node");

	const char* args[2] = {"./a.out", "--nodes=6"};
	const int argc = 2;
	parseCLIArgumentsWithCLIParser(parser, argc, args);

	info("nodes is %d", nodes);
	assert(nodes == 6);

	destroyCLIParser(parser);
}

void testArgParser03(CuTest* tc) {
	int nodes;
	int edges;

	CLIParser* parser = initCLIParser();

	addOptionalIntOptionToCLIParser(parser, 'n', "nodes", 5, &nodes, "node");
	addOptionalIntOptionToCLIParser(parser, 'e', "edges", 6, &edges, "edges");

	const char* args[1] = {"Example", "-e10"};
	const int argc = 2;
	info("args[0] is %s", args[0]);
	parseCLIArgumentsWithCLIParser(parser, argc, args);

	assert(nodes == 5);

	destroyCLIParser(parser);
}

void testArgParser04(CuTest* tc) {
	const char* algorithm;

	CLIParser* parser = initCLIParser();

	addOptionalIntOptionToCLIParser(parser, 'a', "algorithm", "PC1", &algorithm, "either PC1,PC2 or PC3");

	const char* args[] = {"Example", "-a\"PC2\""};
	const int argc = 1;
	parseCLIArgumentsWithCLIParser(parser, argc, args);

	assert(strcmp(algorithm, "PC2") == 0);

	destroyCLIParser(parser);
}

CuSuite* CuArgParserSuite() {
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, testArgParser01);
	SUITE_ADD_TEST(suite, testArgParser02);
	SUITE_ADD_TEST(suite, testArgParser03);
//	SUITE_ADD_TEST(suite, testArgParser04);

	return suite;
}
