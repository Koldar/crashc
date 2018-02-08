/*
 * graphTest.c
 *
 *  Created on: May 18, 2017
 *      Author: koldar
 */

#define _GNU_SOURCE

#include <stdio.h>
#include "CuTest.h"
#include "edge.h"
#include "node.h"
#include "log.h"
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "predsuccgraph.h"
#include "defaultFunctions.h"

int LEQ =	0x00000110;
int EQ =	0x00000010;
int NOTEQ = 0x00000101;
int LT =	0x00000100;

static Edge* contextFindEdgeByBounds = NULL;
static bool lambdaFindEdgeByBounds(Edge* e) {
	return	(e->source->id == contextFindEdgeByBounds->source->id) &&
			(e->sink->id == contextFindEdgeByBounds->sink->id);
}


static Node* contextFindEdgeByNodesSource;
static Node* contextFindEdgeByNodesSink;
static bool lambdaFindEdgeByNodes(Edge* e) {
	return (e->source->id == contextFindEdgeByNodesSource->id) && (e->sink->id == contextFindEdgeByNodesSink->id);
}

static Edge* contextFindEdgeBySinkAndLabel = NULL;
static bool lambdaFindEdgeBySinkAndLabel(Edge* e) {
	info("payloads: e %d context %d", *(int*)e->payload, *(int*)contextFindEdgeBySinkAndLabel->payload);
	if (*getEdgePayloadAs(e, int*) != *getEdgePayloadAs(contextFindEdgeBySinkAndLabel, int*)) {
		return false;
	}
	if (e->sink->id != contextFindEdgeBySinkAndLabel->sink->id) {
		return false;
	}
	return true;
}

static bool compare2Integer(int* a, int* b) {
	return *a == *b;
}

static void destroyInteger(int* n) {
	if (n != NULL) {
		free(n);
	}
}

static int* cloneInteger(int* n) {
	if (n != NULL) {
		int* retVal = malloc(sizeof(int));
		*retVal = *n;
		return retVal;
	}
	return NULL;
}

static const char* stringifyInteger(int* n) {
	if (n == NULL) {
		return NULL;
	}
	char* retVal;
	asprintf(&retVal, "%d", *n);
	return retVal;
}

void TestDrawGraph(CuTest* tc) {

	PredSuccGraph* g = initPredSuccGraph();
	Node* n0 = initNode(0);
	Node* n1 = initNode(1);
	Node* n2 = initNode(2);
	Node* n3 = initNode(3);
	Node* n4 = initNode(4);
	Node* n5 = initNode(5);
	Node* n6 = initNode(6);
	Node* n7 = initNode(7);

	addNodeInGraph(g, n0);
	addNodeInGraph(g, n1);
	addNodeInGraph(g, n2);
	addNodeInGraph(g, n3);
	addNodeInGraph(g, n4);
	addNodeInGraph(g, n5);
	addNodeInGraph(g, n6);
	addNodeInGraph(g, n7);

	int* p;
	//first SCC
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n0, n1, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n1, n2, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n2, n3, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n3, n4, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n4, n5, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n5, n0, p);
	p = malloc(sizeof(int)); *p=EQ; addEdgeInGraph(n1, n4, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n2, n5, p);
	p = malloc(sizeof(int)); *p=EQ; addEdgeInGraph(n1, n5, p);

	//second SCC
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n6, n7, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n7, n6, p);

	//edge between SCC
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n0, n6, p);
	p = malloc(sizeof(int)); *p=EQ; addEdgeInGraph(n2, n6, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n3, n7, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n4, n7, p);

	printGraph(g, __func__);

	destroyPredSuccGraph(g);
}

void testgetEdgeInGraphById01(CuTest* tc) {
	PredSuccGraph* g = initPredSuccGraph();
	Node* n0 = initNode(0);
	Node* n1 = initNode(1);
	Node* n2 = initNode(2);
	Node* n3 = initNode(3);
	Node* n4 = initNode(4);
	Node* n5 = initNode(5);
	Node* n6 = initNode(6);
	Node* n7 = initNode(7);

	addNodeInGraph(g, n0);
	addNodeInGraph(g, n1);
	addNodeInGraph(g, n2);
	addNodeInGraph(g, n3);
	addNodeInGraph(g, n4);
	addNodeInGraph(g, n5);
	addNodeInGraph(g, n6);
	addNodeInGraph(g, n7);

	int* p;
	//first SCC
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n0, n1, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n1, n2, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n2, n3, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n3, n4, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n4, n5, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n5, n0, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n1, n4, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n2, n5, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n1, n5, p);

	//second SCC
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n6, n7, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n7, n6, p);

	//edge between SCC
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n0, n6, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n2, n6, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n3, n7, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n4, n7, p);

	printGraph(g, __func__);

	assert(getEdgeInGraphById(g, n0->id, n1->id) != NULL);
	assert(getEdgeInGraphById(g, n2->id, n3->id) != NULL);
	assert(getEdgeInGraphById(g, n6->id, n7->id) != NULL);
	//non existent edges
	assert(getEdgeInGraphById(g, n6->id, n0->id) == NULL);
	assert(getEdgeInGraphById(g, n0->id, n3->id) == NULL);
	assert(getEdgeInGraphById(g, n3->id, n0->id) == NULL);

	destroyPredSuccGraph(g);
}

///compute the consistency of a graph with all non-flawed edges
void testcheckPredSuccGraphDestruction(CuTest* tc) {
	PredSuccGraph* g = initPredSuccGraph();
	Node* n0 = initNode(0);
	Node* n1 = initNode(1);
	Node* n2 = initNode(2);
	Node* n3 = initNode(3);
	Node* n4 = initNode(4);
	Node* n5 = initNode(5);
	Node* n6 = initNode(6);
	Node* n7 = initNode(7);

	addNodeInGraph(g, n0);
	addNodeInGraph(g, n1);
	addNodeInGraph(g, n2);
	addNodeInGraph(g, n3);
	addNodeInGraph(g, n4);
	addNodeInGraph(g, n5);
	addNodeInGraph(g, n6);
	addNodeInGraph(g, n7);

	int* p;
	//first SCC
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n0, n1, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n1, n2, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n2, n3, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n3, n4, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n4, n5, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n5, n0, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n0, n3, p);
	p = malloc(sizeof(int)); *p=EQ; addEdgeInGraph(n1, n4, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n2, n5, p);
	p = malloc(sizeof(int)); *p=EQ; addEdgeInGraph(n1, n5, p);

	//second SCC
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n6, n7, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n7, n6, p);

	//edge between SCC
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n0, n6, p);
	p = malloc(sizeof(int)); *p=EQ; addEdgeInGraph(n2, n6, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n3, n7, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n4, n7, p);

	printGraph(g, __func__);

	destroyPredSuccGraph(g);
}

///the 2 graphs have different sizes
void testpredSuccGraphCmp01(CuTest* tc) {
	PredSuccGraph* g = initPredSuccGraph();
	Node* n0 = initNode(0);
	Node* n1 = initNode(1);
	Node* n2 = initNode(2);
	Node* n3 = initNode(3);
	Node* n4 = initNode(4);
	Node* n5 = initNode(5);
	Node* n6 = initNode(6);
	Node* n7 = initNode(7);

	addNodeInGraph(g, n0);
	addNodeInGraph(g, n1);
	addNodeInGraph(g, n2);
	addNodeInGraph(g, n3);
	addNodeInGraph(g, n4);
	addNodeInGraph(g, n5);
	addNodeInGraph(g, n6);
	addNodeInGraph(g, n7);


	g->edgePayloadComparator = compare2Integer;

	int* p;
	//first SCC
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n0, n1, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n1, n2, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n2, n3, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n3, n4, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n4, n5, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n5, n0, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n0, n3, p);
	p = malloc(sizeof(int)); *p=EQ; addEdgeInGraph(n1, n4, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n2, n5, p);
	p = malloc(sizeof(int)); *p=EQ; addEdgeInGraph(n1, n5, p);

	//second SCC
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n6, n7, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n7, n6, p);

	//edge between SCC
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n0, n6, p);
	p = malloc(sizeof(int)); *p=EQ; addEdgeInGraph(n2, n6, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n3, n7, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n4, n7, p);

	PredSuccGraph* g2 = initPredSuccGraph();
	Node* n02 = initNode(0);
	Node* n12 = initNode(1);
	Node* n22 = initNode(2);
	Node* n32 = initNode(3);
	Node* n42 = initNode(4);
	Node* n52 = initNode(5);
	Node* n62 = initNode(6);

	addNodeInGraph(g2, n02);
	addNodeInGraph(g2, n12);
	addNodeInGraph(g2, n22);
	addNodeInGraph(g2, n32);
	addNodeInGraph(g2, n42);
	addNodeInGraph(g2, n52);
	addNodeInGraph(g2, n62);

	g2->edgePayloadComparator = compare2Integer;
	//first SCC
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n02, n12, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n12, n22, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n22, n32, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n32, n42, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n42, n52, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n52, n02, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n02, n32, p);
	p = malloc(sizeof(int)); *p=EQ; addEdgeInGraph(n12, n42, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n22, n52, p);
	p = malloc(sizeof(int)); *p=EQ; addEdgeInGraph(n12, n52, p);

	//edge between SCC
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n02, n62, p);
	p = malloc(sizeof(int)); *p=EQ; addEdgeInGraph(n22, n62, p);

	assert(predSuccGraphCmp(g, g2) == false);

	destroyPredSuccGraph(g);
	destroyPredSuccGraph(g2);
}

///the 2 graphs are the same, but the second one does not have an edge
void testpredSuccGraphCmp02(CuTest* tc) {
	PredSuccGraph* g = initPredSuccGraph();
	Node* n0 = initNode(0);
	Node* n1 = initNode(1);
	Node* n2 = initNode(2);
	Node* n3 = initNode(3);
	Node* n4 = initNode(4);
	Node* n5 = initNode(5);
	Node* n6 = initNode(6);
	Node* n7 = initNode(7);

	addNodeInGraph(g, n0);
	addNodeInGraph(g, n1);
	addNodeInGraph(g, n2);
	addNodeInGraph(g, n3);
	addNodeInGraph(g, n4);
	addNodeInGraph(g, n5);
	addNodeInGraph(g, n6);
	addNodeInGraph(g, n7);

	g->edgePayloadComparator = compare2Integer;

	int* p;
	//first SCC
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n0, n1, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n1, n2, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n2, n3, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n3, n4, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n4, n5, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n5, n0, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n0, n3, p);
	p = malloc(sizeof(int)); *p=EQ; addEdgeInGraph(n1, n4, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n2, n5, p);
	p = malloc(sizeof(int)); *p=EQ; addEdgeInGraph(n1, n5, p);

	//second SCC
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n6, n7, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n7, n6, p);

	//edge between SCC
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n0, n6, p);
	p = malloc(sizeof(int)); *p=EQ; addEdgeInGraph(n2, n6, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n3, n7, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n4, n7, p);

	PredSuccGraph* g2 = initPredSuccGraph();
	Node* n02 = initNode(0);
	Node* n12 = initNode(1);
	Node* n22 = initNode(2);
	Node* n32 = initNode(3);
	Node* n42 = initNode(4);
	Node* n52 = initNode(5);
	Node* n62 = initNode(6);
	Node* n72 = initNode(7);

	addNodeInGraph(g2, n02);
	addNodeInGraph(g2, n12);
	addNodeInGraph(g2, n22);
	addNodeInGraph(g2, n32);
	addNodeInGraph(g2, n42);
	addNodeInGraph(g2, n52);
	addNodeInGraph(g2, n62);
	addNodeInGraph(g2, n72);

	g2->edgePayloadComparator = compare2Integer;
	//first SCC
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n02, n12, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n12, n22, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n22, n32, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n32, n42, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n42, n52, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n52, n02, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n02, n32, p);
	p = malloc(sizeof(int)); *p=EQ; addEdgeInGraph(n12, n42, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n22, n52, p);
	p = malloc(sizeof(int)); *p=EQ; addEdgeInGraph(n12, n52, p);

	//second SCC
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n62, n72, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n72, n62, p);

	//edge between SCC
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n02, n62, p);
	p = malloc(sizeof(int)); *p=EQ; addEdgeInGraph(n22, n62, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n32, n72, p);

	assert(predSuccGraphCmp(g, g2) == false);

	destroyPredSuccGraph(g);
	destroyPredSuccGraph(g2);

}

///the 2 graphs are the same but a label of an edge in the second graph is different
void testpredSuccGraphCmp03(CuTest* tc) {
	PredSuccGraph* g = initPredSuccGraph();
	Node* n0 = initNode(0);
	Node* n1 = initNode(1);
	Node* n2 = initNode(2);
	Node* n3 = initNode(3);
	Node* n4 = initNode(4);
	Node* n5 = initNode(5);
	Node* n6 = initNode(6);
	Node* n7 = initNode(7);

	addNodeInGraph(g, n0);
	addNodeInGraph(g, n1);
	addNodeInGraph(g, n2);
	addNodeInGraph(g, n3);
	addNodeInGraph(g, n4);
	addNodeInGraph(g, n5);
	addNodeInGraph(g, n6);
	addNodeInGraph(g, n7);

	g->edgePayloadComparator = compare2Integer;

	int* p;
	//first SCC
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n0, n1, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n1, n2, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n2, n3, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n3, n4, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n4, n5, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n5, n0, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n0, n3, p);
	p = malloc(sizeof(int)); *p=EQ; addEdgeInGraph(n1, n4, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n2, n5, p);
	p = malloc(sizeof(int)); *p=EQ; addEdgeInGraph(n1, n5, p);

	//second SCC
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n6, n7, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n7, n6, p);

	//edge between SCC
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n0, n6, p);
	p = malloc(sizeof(int)); *p=EQ; addEdgeInGraph(n2, n6, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n3, n7, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n4, n7, p);

	PredSuccGraph* g2 = initPredSuccGraph();
	Node* n02 = initNode(0);
	Node* n12 = initNode(1);
	Node* n22 = initNode(2);
	Node* n32 = initNode(3);
	Node* n42 = initNode(4);
	Node* n52 = initNode(5);
	Node* n62 = initNode(6);
	Node* n72 = initNode(7);

	addNodeInGraph(g2, n02);
	addNodeInGraph(g2, n12);
	addNodeInGraph(g2, n22);
	addNodeInGraph(g2, n32);
	addNodeInGraph(g2, n42);
	addNodeInGraph(g2, n52);
	addNodeInGraph(g2, n62);
	addNodeInGraph(g2, n72);

	g2->edgePayloadComparator = compare2Integer;
	//first SCC
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n02, n12, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n12, n22, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n22, n32, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n32, n42, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n42, n52, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n52, n02, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n02, n32, p);
	p = malloc(sizeof(int)); *p=EQ; addEdgeInGraph(n12, n42, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n22, n52, p);
	p = malloc(sizeof(int)); *p=EQ; addEdgeInGraph(n12, n52, p);

	//second SCC
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n62, n72, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n72, n62, p);

	//edge between SCC
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n02, n62, p);
	p = malloc(sizeof(int)); *p=EQ; addEdgeInGraph(n22, n62, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n32, n72, p);
	p = malloc(sizeof(int)); *p=LT; addEdgeInGraph(n42, n72, p);

	assert(predSuccGraphCmp(g, g2) == false);

	destroyPredSuccGraph(g);
	destroyPredSuccGraph(g2);
}

///the 2 graphs are the same
void testpredSuccGraphCmp04(CuTest* tc) {
	PredSuccGraph* g = initPredSuccGraph();
	Node* n0 = initNode(0);
	Node* n1 = initNode(1);
	Node* n2 = initNode(2);
	Node* n3 = initNode(3);
	Node* n4 = initNode(4);
	Node* n5 = initNode(5);
	Node* n6 = initNode(6);
	Node* n7 = initNode(7);

	addNodeInGraph(g, n0);
	addNodeInGraph(g, n1);
	addNodeInGraph(g, n2);
	addNodeInGraph(g, n3);
	addNodeInGraph(g, n4);
	addNodeInGraph(g, n5);
	addNodeInGraph(g, n6);
	addNodeInGraph(g, n7);

	g->edgePayloadComparator = compare2Integer;

	int* p;
	//first SCC
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n0, n1, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n1, n2, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n2, n3, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n3, n4, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n4, n5, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n5, n0, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n0, n3, p);
	p = malloc(sizeof(int)); *p=EQ; addEdgeInGraph(n1, n4, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n2, n5, p);
	p = malloc(sizeof(int)); *p=EQ; addEdgeInGraph(n1, n5, p);

	//second SCC
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n6, n7, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n7, n6, p);

	//edge between SCC
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n0, n6, p);
	p = malloc(sizeof(int)); *p=EQ; addEdgeInGraph(n2, n6, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n3, n7, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n4, n7, p);

	PredSuccGraph* g2 = initPredSuccGraph();
	Node* n02 = initNode(0);
	Node* n12 = initNode(1);
	Node* n22 = initNode(2);
	Node* n32 = initNode(3);
	Node* n42 = initNode(4);
	Node* n52 = initNode(5);
	Node* n62 = initNode(6);
	Node* n72 = initNode(7);

	addNodeInGraph(g2, n02);
	addNodeInGraph(g2, n12);
	addNodeInGraph(g2, n22);
	addNodeInGraph(g2, n32);
	addNodeInGraph(g2, n42);
	addNodeInGraph(g2, n52);
	addNodeInGraph(g2, n62);
	addNodeInGraph(g2, n72);

	g2->edgePayloadComparator = compare2Integer;
	//first SCC
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n02, n12, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n12, n22, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n22, n32, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n32, n42, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n42, n52, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n52, n02, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n02, n32, p);
	p = malloc(sizeof(int)); *p=EQ; addEdgeInGraph(n12, n42, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n22, n52, p);
	p = malloc(sizeof(int)); *p=EQ; addEdgeInGraph(n12, n52, p);

	//second SCC
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n62, n72, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n72, n62, p);

	//edge between SCC
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n02, n62, p);
	p = malloc(sizeof(int)); *p=EQ; addEdgeInGraph(n22, n62, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n32, n72, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n42, n72, p);

	assert(predSuccGraphCmp(g, g2) == true);

	destroyPredSuccGraph(g);
	destroyPredSuccGraph(g2);
}


static bool compare2Integers(int* a, int* b) {
	return *a == *b;
}

void testgetEdgeListOfPredSuccGraph01(CuTest* tc) {
	PredSuccGraph* graph = initPredSuccGraph();
	Node* n0 = initNode(0);
	Node* n1 = initNode(1);
	Node* n2 = initNode(2);
	Node* n3 = initNode(3);
	Node* n4 = initNode(4);
	Node* n5 = initNode(5);
	Node* n6 = initNode(6);
	Node* n7 = initNode(7);

	addNodeInGraph(graph, n0);
	addNodeInGraph(graph, n1);
	addNodeInGraph(graph, n2);
	addNodeInGraph(graph, n3);
	addNodeInGraph(graph, n4);
	addNodeInGraph(graph, n5);
	addNodeInGraph(graph, n6);
	addNodeInGraph(graph, n7);

	int* p;
	//first SCC
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n0, n1, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n1, n2, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n2, n3, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n3, n4, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n4, n5, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n5, n0, p);
	p = malloc(sizeof(int)); *p=LT; addEdgeInGraph(n0, n3, p);
	p = malloc(sizeof(int)); *p=NOTEQ; addEdgeInGraph(n1, n4, p);
	p = malloc(sizeof(int)); *p=LT; addEdgeInGraph(n2, n5, p);
	p = malloc(sizeof(int)); *p=NOTEQ; addEdgeInGraph(n1, n5, p);

	//second SCC
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n6, n7, p);
	p = malloc(sizeof(int)); *p=LT; addEdgeInGraph(n7, n6, p);

	//edge between SCC
	p = malloc(sizeof(int)); *p=LT; addEdgeInGraph(n0, n6, p);
	p = malloc(sizeof(int)); *p=NOTEQ; addEdgeInGraph(n2, n6, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n3, n7, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n4, n7, p);


	EdgeList* list = getEdgeListOfPredSuccGraph(graph);
	assert(getLengthOfList(list) == 16);

	contextFindEdgeByBounds = getEdgeInGraph(n0, n1);	assert( findElementInList(list, lambdaFindEdgeByBounds) != NULL);
	contextFindEdgeByBounds = getEdgeInGraph(n1, n2);	assert( findElementInList(list,lambdaFindEdgeByBounds) != NULL);
	contextFindEdgeByBounds = getEdgeInGraph(n2, n3);	assert( findElementInList(list, lambdaFindEdgeByBounds) != NULL);
	contextFindEdgeByBounds = getEdgeInGraph(n3, n4);	assert( findElementInList(list, lambdaFindEdgeByBounds) != NULL);
	contextFindEdgeByBounds = getEdgeInGraph(n4, n5);	assert( findElementInList(list, lambdaFindEdgeByBounds) != NULL);
	contextFindEdgeByBounds = getEdgeInGraph(n5, n0);	assert( findElementInList(list, lambdaFindEdgeByBounds) != NULL);
	contextFindEdgeByBounds = getEdgeInGraph(n0, n3);	assert( findElementInList(list, lambdaFindEdgeByBounds) != NULL);
	contextFindEdgeByBounds = getEdgeInGraph(n1, n4);	assert( findElementInList(list, lambdaFindEdgeByBounds) != NULL);
	contextFindEdgeByBounds = getEdgeInGraph(n2, n5);	assert( findElementInList(list, lambdaFindEdgeByBounds) != NULL);
	contextFindEdgeByBounds = getEdgeInGraph(n1, n5);	assert( findElementInList(list, lambdaFindEdgeByBounds) != NULL);

	contextFindEdgeByBounds = getEdgeInGraph(n6, n7);	assert( findElementInList(list, lambdaFindEdgeByBounds) != NULL);
	contextFindEdgeByBounds = getEdgeInGraph(n7, n6);	assert( findElementInList(list, lambdaFindEdgeByBounds) != NULL);

	contextFindEdgeByBounds = getEdgeInGraph(n0, n6);	assert( findElementInList(list, lambdaFindEdgeByBounds) != NULL);
	contextFindEdgeByBounds = getEdgeInGraph(n2, n6);	assert( findElementInList(list, lambdaFindEdgeByBounds) != NULL);
	contextFindEdgeByBounds = getEdgeInGraph(n3, n7);	assert( findElementInList(list, lambdaFindEdgeByBounds) != NULL);
	contextFindEdgeByBounds = getEdgeInGraph(n4, n7);	assert( findElementInList(list, lambdaFindEdgeByBounds) != NULL);

	//NULL edges
	contextFindEdgeByNodesSource = n0; contextFindEdgeByNodesSink = n4; assert( findElementInList(list, lambdaFindEdgeByNodes) == NULL);
	contextFindEdgeByNodesSource = n0; contextFindEdgeByNodesSink = n5; assert( findElementInList(list, lambdaFindEdgeByNodes) == NULL);
	contextFindEdgeByNodesSource = n0; contextFindEdgeByNodesSink = n7; assert( findElementInList(list, lambdaFindEdgeByNodes) == NULL);
	contextFindEdgeByNodesSource = n1; contextFindEdgeByNodesSink = n3; assert( findElementInList(list, lambdaFindEdgeByNodes) == NULL);

	destroyList(list);
	destroyPredSuccGraph(graph);
}

void testclonePredSuccGraph01(CuTest* tc) {
	PredSuccGraph* graph = initPredSuccGraph();
	graph->edgePayloadDestructor = destroyInteger;
	graph->edgePayloadCloner = cloneInteger;
	graph->edgePayloadStringify = stringifyInteger;

	Node* n0 = initNode(0);
	Node* n1 = initNode(1);
	Node* n2 = initNode(2);
	Node* n3 = initNode(3);

	addNodeInGraph(graph, n0);
	addNodeInGraph(graph, n1);
	addNodeInGraph(graph, n2);
	addNodeInGraph(graph, n3);


	int* p;
	//first SCC
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n0, n1, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n1, n2, p);
	p = malloc(sizeof(int)); *p=LEQ; addEdgeInGraph(n2, n3, p);
	p = malloc(sizeof(int)); *p=LT; addEdgeInGraph(n0, n3, p);
	p = malloc(sizeof(int)); *p=LT; addEdgeInGraph(n3, n0, p);

	debug("cloning...");
	PredSuccGraph* clone = clonePredSuccGraph(graph);
	debug("done cloning");

	//pointer are differents
	assert(clone != graph);
	assert(getNodeInGraph(graph, 0) != getNodeInGraph(clone, 0));
	assert(getNodeInGraph(graph, 1) != getNodeInGraph(clone, 1));
	assert(getNodeInGraph(graph, 2) != getNodeInGraph(clone, 2));
	assert(getNodeInGraph(graph, 3) != getNodeInGraph(clone, 3));

	assert(getSizeOfGraph(graph) == getSizeOfGraph(clone));

	for (NodeId id=0; id<graph->size; id++) {
		Node* tmp = getNodeInGraph(graph, id);
		Node* tmp2 = getNodeInGraph(clone, id);

		EdgeList* list1 = getEdgeListOfNode(tmp);
		EdgeList* list2 = getEdgeListOfNode(tmp2);

		assert(getLengthOfList(list1) == getLengthOfList(list2));
		assert(tmp != tmp2);
		assert(tmp->id == tmp2->id);
		debug("getForwardListSize(list1) = %d", getLengthOfList(list1));

		assert(list1 != list2);
		char* edgeStr;

		ITERATE_ON_LIST(list1, cell, e, Edge) {
			//in list pointers are different but the sink ids are the same
			contextFindEdgeBySinkAndLabel = e;
			Edge* found = findElementInList(list2, lambdaFindEdgeBySinkAndLabel);
			edgeStr = getStringOfEdge(e, stringifyInteger);
			info("checking if we have found %s in clone", edgeStr);
			free(edgeStr);
			assert(found != NULL);
			info("payload. edge 1: %p, edge 2: %p", contextFindEdgeBySinkAndLabel->payload, found->payload);
			assert(compare2Integers(contextFindEdgeBySinkAndLabel->payload, found->payload));
			assert(contextFindEdgeBySinkAndLabel->sink != found->sink);
			assert(contextFindEdgeBySinkAndLabel->sink->id == found->sink->id);
		}

		destroyList(list1);
		destroyList(list2);
	}

	destroyPredSuccGraph(graph);
	destroyPredSuccGraph(clone);
	info("all destoryed!");
}

void testSerialization01(CuTest* tc) {
	PredSuccGraph* g = initPredSuccGraph();
	g->edgePayloadDestructor = payloadDestructorNOP;
	g->nodePayloadDestructor = payloadDestructorNOP;

	for (int i=0; i<5; i++) {
		addNewNodeInGraph(g, i, i);
	}

	addEdgeInGraphById(g, 0, 4, 7);
	addEdgeInGraphById(g, 0, 3, 8);
	addEdgeInGraphById(g, 0, 2, 9);
	addEdgeInGraphById(g, 3, 0, 10);
	addEdgeInGraphById(g, 3, 1, 11);
	addEdgeInGraphById(g, 4, 3, 12);

	FILE* f = fopen(__func__, "wb");
	storeGraphInFile(f, g);
	fclose(f);
	destroyPredSuccGraph(g);

	f = fopen(__func__, "rb");
	g = loadGraphFromFile(f, defaultDeserializer, defaultDeserializer);
	fclose(f);
	g->edgePayloadDestructor = payloadDestructorNOP;
	g->nodePayloadDestructor = payloadDestructorNOP;

	assert(getSizeOfGraph(g) == 5);

	assert(getEdgeInGraphById(g, 0, 1) == NULL);
	assert(getEdgeInGraphById(g, 0, 2) != NULL);
	assert(getEdgeInGraphById(g, 0, 3) != NULL);
	assert(getEdgeInGraphById(g, 0, 4) != NULL);

	assert(getEdgeInGraphById(g, 1, 0) == NULL);
	assert(getEdgeInGraphById(g, 1, 2) == NULL);
	assert(getEdgeInGraphById(g, 1, 3) == NULL);
	assert(getEdgeInGraphById(g, 1, 4) == NULL);

	assert(getEdgeInGraphById(g, 2, 0) == NULL);
	assert(getEdgeInGraphById(g, 2, 1) == NULL);
	assert(getEdgeInGraphById(g, 2, 3) == NULL);
	assert(getEdgeInGraphById(g, 2, 4) == NULL);

	assert(getEdgeInGraphById(g, 3, 0) != NULL);
	assert(getEdgeInGraphById(g, 3, 1) != NULL);
	assert(getEdgeInGraphById(g, 3, 2) == NULL);
	assert(getEdgeInGraphById(g, 3, 4) == NULL);

	assert(getEdgeInGraphById(g, 4, 0) == NULL);
	assert(getEdgeInGraphById(g, 4, 1) == NULL);
	assert(getEdgeInGraphById(g, 4, 2) == NULL);
	assert(getEdgeInGraphById(g, 4, 3) != NULL);

	assert(getEdgePayloadAs(getEdgeInGraphById(g, 0, 2), int) == 9);
	assert(getEdgePayloadAs(getEdgeInGraphById(g, 0, 3), int) == 8);
	assert(getEdgePayloadAs(getEdgeInGraphById(g, 0, 4), int) == 7);

	assert(getEdgePayloadAs(getEdgeInGraphById(g, 3, 0), int) == 10);
	assert(getEdgePayloadAs(getEdgeInGraphById(g, 3, 1), int) == 11);

	assert(getEdgePayloadAs(getEdgeInGraphById(g, 4, 3), int) == 12);

	assert(getNodePayloadAs(getNodeInGraph(g, 0), int) == 0);
	assert(getNodePayloadAs(getNodeInGraph(g, 1), int) == 1);
	assert(getNodePayloadAs(getNodeInGraph(g, 2), int) == 2);
	assert(getNodePayloadAs(getNodeInGraph(g, 3), int) == 3);
	assert(getNodePayloadAs(getNodeInGraph(g, 4), int) == 4);

	destroyPredSuccGraph(g);
}


CuSuite* CuGraphSuite() {
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, TestDrawGraph);
	SUITE_ADD_TEST(suite, testclonePredSuccGraph01);
	SUITE_ADD_TEST(suite, testgetEdgeListOfPredSuccGraph01);
	SUITE_ADD_TEST(suite, testpredSuccGraphCmp04);
	SUITE_ADD_TEST(suite, testpredSuccGraphCmp03);
	SUITE_ADD_TEST(suite, testpredSuccGraphCmp02);
	SUITE_ADD_TEST(suite, testpredSuccGraphCmp01);
	SUITE_ADD_TEST(suite, testcheckPredSuccGraphDestruction);
	SUITE_ADD_TEST(suite, testgetEdgeInGraphById01);

	SUITE_ADD_TEST(suite, testSerialization01);

	return suite;
}

