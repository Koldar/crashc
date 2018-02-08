/*
 * sccTest.c
 *
 *  Created on: Jun 13, 2017
 *      Author: koldar
 */

#include "CuTest.h"
#include <stdlib.h>
#include <assert.h>
#include "predsuccgraph.h"
#include "log.h"
#include "scc.h"

void testSCC01(CuTest* tc) {
	PredSuccGraph* g = initPredSuccGraph();
	scc_graph* sccGraph = initStronglyConnectedComponentsGraph(g, edge_traverser_alwaysAccept, false, NULL);
	assert(getSizeOfGraph(getStrongConnectedComponentGraph(sccGraph)) == 0);
	destroyStronglyConnectedComponentsGraph(sccGraph);
	destroyPredSuccGraph(g);
}

void testSCC02(CuTest* tc) {
	excludeLogger("redBlackTree.c");

	PredSuccGraph* g = initPredSuccGraph();

	addNewNodeInGraph(g, 0, NULL);
	addNewNodeInGraph(g, 1, NULL);
	addNewNodeInGraph(g, 2, NULL);
	addNewNodeInGraph(g, 3, NULL);
	addNewNodeInGraph(g, 4, NULL);

	addEdgeInGraphById(g, 0, 1, NULL);
	addEdgeInGraphById(g, 1, 2, NULL);
	addEdgeInGraphById(g, 2, 0, NULL);

	addEdgeInGraphById(g, 3, 4, NULL);
	addEdgeInGraphById(g, 4, 3, NULL);

	scc_graph* sccGraph = initStronglyConnectedComponentsGraph(g, edge_traverser_alwaysAccept, false, NULL);

	info("number of scc inside the graph are %d", getSizeOfGraph(getStrongConnectedComponentGraph(sccGraph)));
	assert(getSizeOfGraph(getStrongConnectedComponentGraph(sccGraph)) == 2);

	PredSuccGraph* sccs = getStrongConnectedComponentGraph(sccGraph);
	assert(isNodeInsideStrongConnectedComponentById(sccGraph, 0, 0));
	assert(isNodeInsideStrongConnectedComponentById(sccGraph, 1, 0));
	assert(isNodeInsideStrongConnectedComponentById(sccGraph, 2, 0));

	assert(isNodeInsideStrongConnectedComponentById(sccGraph, 3, 1));
	assert(isNodeInsideStrongConnectedComponentById(sccGraph, 4, 1));

	destroyStronglyConnectedComponentsGraph(sccGraph);
	destroyPredSuccGraph(g);
}

void testSCC03(CuTest* tc) {
	excludeLogger("redBlackTree.c");

	PredSuccGraph* g = initPredSuccGraph();

	addNewNodeInGraph(g, 0, NULL);
	addNewNodeInGraph(g, 1, NULL);
	addNewNodeInGraph(g, 2, NULL);
	addNewNodeInGraph(g, 3, NULL);
	addNewNodeInGraph(g, 4, NULL);

	addEdgeInGraphById(g, 0, 1, NULL);
	addEdgeInGraphById(g, 1, 2, NULL);
	addEdgeInGraphById(g, 2, 0, NULL);

	addEdgeInGraphById(g, 3, 4, NULL);
	addEdgeInGraphById(g, 4, 3, NULL);

	addEdgeInGraphById(g, 4, 2, NULL);
	addEdgeInGraphById(g, 4, 0, NULL);
	addEdgeInGraphById(g, 3, 1, NULL);

	scc_graph* sccGraph = initStronglyConnectedComponentsGraph(g, edge_traverser_alwaysAccept, true, NULL);

	info("number of scc inside the graph are %d", getSizeOfGraph(getStrongConnectedComponentGraph(sccGraph)));
	assert(getSizeOfGraph(getStrongConnectedComponentGraph(sccGraph)) == 2);

	PredSuccGraph* sccs = getStrongConnectedComponentGraph(sccGraph);
	assert(isNodeInsideStrongConnectedComponentById(sccGraph, 0, 0));
	assert(isNodeInsideStrongConnectedComponentById(sccGraph, 1, 0));
	assert(isNodeInsideStrongConnectedComponentById(sccGraph, 2, 0));

	assert(isNodeInsideStrongConnectedComponentById(sccGraph, 3, 1));
	assert(isNodeInsideStrongConnectedComponentById(sccGraph, 4, 1));

	int sum = 0;
	Node* scc0 = getNodeInGraph(getStrongConnectedComponentGraph(sccGraph), 0);
	assert(scc0 != NULL);
	assert(getNumberOfNodesInsideStrongConnectedComponent(scc0) == 3);
	ITERATE_VALUES_ON_HT(&getStrongConnectedComponentOfNodeById(sccGraph, 0)->successors, e1, Edge) {
		EdgeList* l = getEdgePayloadAs(e1, EdgeList*);
		sum += getLengthOfList(l);
	}
	assert(sum == 0);

	sum = 0;
	int edgesNumber = 0;
	Node* scc1 = getNodeInGraph(getStrongConnectedComponentGraph(sccGraph), 1);
	assert(scc1 != NULL);
	info("getNumberOfNodesInsideSCC(scc1) == %d", getNumberOfNodesInsideStrongConnectedComponent(scc1));
	assert(getNumberOfNodesInsideStrongConnectedComponent(scc1) == 2);
	ITERATE_VALUES_ON_HT(&scc1->successors, e2, Edge) {
		edgesNumber += 1;
		EdgeList* l = getEdgePayloadAs(e2, EdgeList*);
		sum += getLengthOfList(l);
	}
	info("sum is %d; edgesNumber is %d", sum, edgesNumber);
	assert(edgesNumber == 1);
	assert(sum == 3);

	destroyStronglyConnectedComponentsGraph(sccGraph);
	destroyPredSuccGraph(g);
}

void testSCC04(CuTest* tc) {
	excludeLogger("redBlackTree.c");

	PredSuccGraph* g = initPredSuccGraph();

	addNewNodeInGraph(g, 0, NULL);
	addNewNodeInGraph(g, 1, NULL);
	addNewNodeInGraph(g, 2, NULL);

	addEdgeInGraphById(g, 0, 1, NULL);
	addEdgeInGraphById(g, 1, 2, NULL);

	scc_graph* sccGraph = initStronglyConnectedComponentsGraph(g, edge_traverser_alwaysAccept, true, NULL);

	info("number of scc inside the graph are %d", getSizeOfGraph(getStrongConnectedComponentGraph(sccGraph)));
	assert(getSizeOfGraph(getStrongConnectedComponentGraph(sccGraph)) == 3);

	PredSuccGraph* sccs = getStrongConnectedComponentGraph(sccGraph);
	assert(getNumberOfNodesInsideStrongConnectedComponent(getStrongConnectedComponentOfNodeById(sccGraph, 0)) == 1);
	assert(getNumberOfNodesInsideStrongConnectedComponent(getStrongConnectedComponentOfNodeById(sccGraph, 1)) == 1);
	assert(getNumberOfNodesInsideStrongConnectedComponent(getStrongConnectedComponentOfNodeById(sccGraph, 2)) == 1);

	int sum = 0;
	int edgesNumber = 0;

	Node* scc = getStrongConnectedComponentOfNodeById(sccGraph, 0);
	assert(scc != NULL);
	assert(getNumberOfNodesInsideStrongConnectedComponent(scc) == 1);
	ITERATE_VALUES_ON_HT(&scc->successors, e1, Edge) {
		edgesNumber += 1;
		EdgeList* l = getEdgePayloadAs(e1, EdgeList*);
		sum += getLengthOfList(l);
	}
	debug("sum is %d", sum);
	assert(sum == 1);
	assert(edgesNumber == 1);

	sum = 0;
	edgesNumber = 0;
	scc = getStrongConnectedComponentOfNodeById(sccGraph, 1);
	assert(scc != NULL);
	assert(getNumberOfNodesInsideStrongConnectedComponent(scc) == 1);
	ITERATE_VALUES_ON_HT(&scc->successors, e2, Edge) {
		edgesNumber += 1;
		EdgeList* l = getEdgePayloadAs(e2, EdgeList*);
		sum += getLengthOfList(l);
	}
	assert(sum == 1);
	assert(edgesNumber == 1);

	sum = 0;
	edgesNumber = 0;
	scc = getStrongConnectedComponentOfNodeById(sccGraph, 2);
	assert(scc != NULL);
	assert(getNumberOfNodesInsideStrongConnectedComponent(scc) == 1);
	ITERATE_VALUES_ON_HT(&scc->successors, e3, Edge) {
		edgesNumber += 1;
		EdgeList* l = getEdgePayloadAs(e3, EdgeList*);
		sum += getLengthOfList(l);
	}
	assert(sum == 0);
	assert(edgesNumber == 0);


	destroyStronglyConnectedComponentsGraph(sccGraph);
	destroyPredSuccGraph(g);
}

CuSuite* CuSCCSuite() {
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, testSCC01);
	SUITE_ADD_TEST(suite, testSCC02);
	SUITE_ADD_TEST(suite, testSCC03);
	SUITE_ADD_TEST(suite, testSCC04);


	return suite;
}

