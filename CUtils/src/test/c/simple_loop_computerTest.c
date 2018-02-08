/*
 * simple_loop_computerTest.c
 *
 *  Created on: Jul 31, 2017
 *      Author: koldar
 */


#include "CuTest.h"
#include <assert.h>
#include "simple_loops_computer.h"
#include "log.h"
#include "defaultFunctions.h"

///no loops
void testSimpleLoopComputer01(CuTest* tc) {
	PredSuccGraph* g = initPredSuccGraph();
	johnson_support* support = initJohnsonSupport();

	addNewNodeInGraph(g, 0, NULL);
	addNewNodeInGraph(g, 1, NULL);
	addNewNodeInGraph(g, 2, NULL);
	addNewNodeInGraph(g, 3, NULL);


	addEdgeInGraphById(g, 0, 1, NULL);
	addEdgeInGraphById(g, 1, 2, NULL);
	addEdgeInGraphById(g, 2, 3, NULL);

	loop_list* simpleLoops = computeSimpleLoopsInGraph(support, g, edge_traverser_alwaysAccept);

	assert(isEmptyList(simpleLoops));

	destroyList(simpleLoops);
	destroyJohnsonSupport(support);
	destroyPredSuccGraph(g);
}

///one loop
void testSimpleLoopComputer02(CuTest* tc) {
	PredSuccGraph* g = initPredSuccGraph();
	johnson_support* support = initJohnsonSupport();

	addNewNodeInGraph(g, 0, NULL);
	addNewNodeInGraph(g, 1, NULL);
	addNewNodeInGraph(g, 2, NULL);
	addNewNodeInGraph(g, 3, NULL);


	addEdgeInGraphById(g, 0, 1, NULL);
	addEdgeInGraphById(g, 1, 2, NULL);
	addEdgeInGraphById(g, 2, 3, NULL);
	addEdgeInGraphById(g, 3, 0, NULL);

	loop_list* simpleLoops = computeSimpleLoopsInGraph(support, g, edge_traverser_alwaysAccept);

	assert(!isEmptyList(simpleLoops));
	critical("getLengthOfList(simpleLoops) = %d", getLengthOfList(simpleLoops));
	assert(getLengthOfList(simpleLoops) == 1);
	loop* l = getNthElementOfList(simpleLoops, 0);
	int sum = 0;
	ITERATE_ON_LIST(l, cell, n, Node) {
		sum += n->id;
	}
	assert(sum == 6);

	destroyList(simpleLoops);
	destroyJohnsonSupport(support);
	destroyPredSuccGraph(g);
}

///2 loops
void testSimpleLoopComputer03(CuTest* tc) {
	PredSuccGraph* g = initPredSuccGraph();
	johnson_support* support = initJohnsonSupport();

	addNewNodeInGraph(g, 0, NULL);
	addNewNodeInGraph(g, 1, NULL);
	addNewNodeInGraph(g, 2, NULL);
	addNewNodeInGraph(g, 3, NULL);


	addEdgeInGraphById(g, 0, 1, NULL);
	addEdgeInGraphById(g, 1, 2, NULL);
	addEdgeInGraphById(g, 2, 3, NULL);
	addEdgeInGraphById(g, 3, 0, NULL);
	addEdgeInGraphById(g, 2, 0, NULL);

	loop_list* simpleLoops = computeSimpleLoopsInGraph(support, g, edge_traverser_alwaysAccept);

	assert(!isEmptyList(simpleLoops));
	critical("getLengthOfList(simpleLoops) = %d", getLengthOfList(simpleLoops));
	assert(getLengthOfList(simpleLoops) == 2);
	loop* l;
	int sum = 0;

	l = getNthElementOfList(simpleLoops, 0);
	sum = 0;
	ITERATE_ON_LIST(l, cell, n, Node) {
		sum += n->id;
	}
	assert(sum == 6);

	l = getNthElementOfList(simpleLoops, 1);
	sum = 0;
	ITERATE_ON_LIST(l, cell2, n2, Node) {
		sum += n2->id;
	}
	assert(sum == 3);

	destroyList(simpleLoops);
	destroyJohnsonSupport(support);
	destroyPredSuccGraph(g);
}

static orderer_by_id(const Node* n1, const Node* n2) {
	return n1->id - n2->id;
}

///test graph in https://youtu.be/johyrWospv0
void testSimpleLoopComputer04(CuTest* tc) {
	PredSuccGraph* g = initPredSuccGraph();
	johnson_support* support = initJohnsonSupport();

	for (NodeId i=0; i<9; i++) {
		addNewNodeInGraph(g, i, NULL);
	}

	addEdgeInGraphById(g, 0, 1, NULL);
	addEdgeInGraphById(g, 0, 4, NULL);
	addEdgeInGraphById(g, 0, 7, NULL);
	addEdgeInGraphById(g, 1, 2, NULL);
	addEdgeInGraphById(g, 1, 8, NULL);
	addEdgeInGraphById(g, 1, 6, NULL);
	addEdgeInGraphById(g, 2, 1, NULL);
	addEdgeInGraphById(g, 2, 0, NULL);
	addEdgeInGraphById(g, 2, 5, NULL);
	addEdgeInGraphById(g, 2, 3, NULL);
	addEdgeInGraphById(g, 3, 4, NULL);
	addEdgeInGraphById(g, 4, 1, NULL);
	addEdgeInGraphById(g, 5, 3, NULL);
	addEdgeInGraphById(g, 7, 8, NULL);
	addEdgeInGraphById(g, 8, 7, NULL);

	loop_list* simpleLoops = computeSimpleLoopsInGraph(support, g, edge_traverser_alwaysAccept);

	assert(!isEmptyList(simpleLoops));
	critical("getLengthOfList(simpleLoops) = %d", getLengthOfList(simpleLoops));
	assert(getLengthOfList(simpleLoops) == 6);
	loop* l;
	node_heap* tmph;
	tmph = initHeap(100, payloadDestructorNOP, orderer_by_id);

	//first cycle
	clearHeap(tmph);
	l = getNthElementOfList(simpleLoops, 0);
	ITERATE_ON_LIST(l, cell1, n1, Node) {
		insertItemInHeap(tmph, n1);
	}
	assert(getSizeOfHeap(tmph) == 3);
	assert(containsItemInHeap(tmph, getNodeInGraph(g, 0)));
	assert(containsItemInHeap(tmph, getNodeInGraph(g, 1)));
	assert(containsItemInHeap(tmph, getNodeInGraph(g, 2)));

	//third cycle
	clearHeap(tmph);
	l = getNthElementOfList(simpleLoops, 1);
	ITERATE_ON_LIST(l, cell3, n3, Node) {
		insertItemInHeap(tmph, n3);
	}
	assert(getSizeOfHeap(tmph) == 4);
	assert(containsItemInHeap(tmph, getNodeInGraph(g, 0)));
	assert(containsItemInHeap(tmph, getNodeInGraph(g, 4)));
	assert(containsItemInHeap(tmph, getNodeInGraph(g, 1)));
	assert(containsItemInHeap(tmph, getNodeInGraph(g, 2)));

	//fourth cycle
	clearHeap(tmph);
	l = getNthElementOfList(simpleLoops, 2);
	ITERATE_ON_LIST(l, cell4, n4, Node) {
		insertItemInHeap(tmph, n4);
	}
	assert(getSizeOfHeap(tmph) == 2);
	assert(containsItemInHeap(tmph, getNodeInGraph(g, 1)));
	assert(containsItemInHeap(tmph, getNodeInGraph(g, 2)));

	//sixth cycle
	clearHeap(tmph);
	l = getNthElementOfList(simpleLoops, 3);
	ITERATE_ON_LIST(l, cell6, n6, Node) {
		insertItemInHeap(tmph, n6);
	}
	assert(getSizeOfHeap(tmph) == 5);
	assert(containsItemInHeap(tmph, getNodeInGraph(g, 1)));
	assert(containsItemInHeap(tmph, getNodeInGraph(g, 2)));
	assert(containsItemInHeap(tmph, getNodeInGraph(g, 5)));
	assert(containsItemInHeap(tmph, getNodeInGraph(g, 3)));
	assert(containsItemInHeap(tmph, getNodeInGraph(g, 4)));

	//fifth cycle
	clearHeap(tmph);
	l = getNthElementOfList(simpleLoops, 4);
	ITERATE_ON_LIST(l, cell5, n5, Node) {
		insertItemInHeap(tmph, n5);
	}
	assert(getSizeOfHeap(tmph) == 4);
	assert(containsItemInHeap(tmph, getNodeInGraph(g, 1)));
	assert(containsItemInHeap(tmph, getNodeInGraph(g, 2)));
	assert(containsItemInHeap(tmph, getNodeInGraph(g, 3)));
	assert(containsItemInHeap(tmph, getNodeInGraph(g, 4)));

	//second cycle
	clearHeap(tmph);
	l = getNthElementOfList(simpleLoops, 5);
	ITERATE_ON_LIST(l, cell2, n2, Node) {
		insertItemInHeap(tmph, n2);
	}
	assert(getSizeOfHeap(tmph) == 2);
	assert(containsItemInHeap(tmph, getNodeInGraph(g, 7)));
	assert(containsItemInHeap(tmph, getNodeInGraph(g, 8)));

	destroyHeap(tmph);
	destroyList(simpleLoops);
	destroyJohnsonSupport(support);
	destroyPredSuccGraph(g);
}

CuSuite* CuSimpleLoopComputerSuite() {
	CuSuite* suite = CuSuiteNew();

	//	SUITE_ADD_TEST(suite, testSimpleLoopComputer01);
	//	SUITE_ADD_TEST(suite, testSimpleLoopComputer02);
	//	SUITE_ADD_TEST(suite, testSimpleLoopComputer03);
	SUITE_ADD_TEST(suite, testSimpleLoopComputer04);


	return suite;
}

