/*
 * topologicalOrderTest.c
 *
 *  Created on: Jun 1, 2017
 *      Author: koldar
 */


#include "CuTest.h"
#include "topologicalOrder.h"
#include <assert.h>
#include "log.h"

void testCormen01(CuSuite* tc) {
	PredSuccGraph* g = initPredSuccGraph();

	addNewNodeInGraph(g, 0, NULL);
	addNewNodeInGraph(g, 1, NULL);
	addNewNodeInGraph(g, 2, NULL);
	addNewNodeInGraph(g, 3, NULL);
	addNewNodeInGraph(g, 4, NULL);

	addEdgeInGraphById(g, 0, 1, NULL);
	addEdgeInGraphById(g, 1, 2, NULL);
	addEdgeInGraphById(g, 2, 3, NULL);
	addEdgeInGraphById(g, 3, 4, NULL);

	NodeList* output = initList();
	doStaticTopologicalOrderWith(TO_CORMEN, g, output);

	int i = 0;
	ITERATE_ON_LIST(output, cell, n, Node) {
		info("node in position %d is %ld", i, n->id);
		switch (i) {
		case 0: assert(n-> id == 0); break;
		case 1: assert(n-> id == 1); break;
		case 2: assert(n-> id == 2); break;
		case 3: assert(n-> id == 3); break;
		case 4: assert(n-> id == 4); break;
		}
		i += 1;
	}

	destroyList(output);
	destroyPredSuccGraph(g);
}

void testCormen02(CuSuite* tc) {
	PredSuccGraph* g = initPredSuccGraph();

	addNewNodeInGraph(g, 0, NULL);
	addNewNodeInGraph(g, 1, NULL);
	addNewNodeInGraph(g, 2, NULL);
	addNewNodeInGraph(g, 3, NULL);
	addNewNodeInGraph(g, 4, NULL);

	addEdgeInGraphById(g, 3, 2, NULL);
	addEdgeInGraphById(g, 2, 4, NULL);
	addEdgeInGraphById(g, 2, 0, NULL);
	addEdgeInGraphById(g, 3, 1, NULL);

	printGraph(g, __func__);

	NodeList* output = initList();
	doStaticTopologicalOrderWith(TO_CORMEN, g, output);

	int i=0;
	ITERATE_ON_LIST(output, cell, node, Node) {
		info("%d-th element of topological sort is %ld", i, node->id);
		i++;
	}
	assert(
			(
					(((Node*)getNthElementOfList(output, 0))->id == 3) &&
					(((Node*)getNthElementOfList(output, 1))->id == 2) &&
					(((Node*)getNthElementOfList(output, 2))->id == 1) &&
					(((Node*)getNthElementOfList(output, 3))->id == 4) &&
					(((Node*)getNthElementOfList(output, 4))->id == 0)
			) || (
					(((Node*)getNthElementOfList(output, 0))->id == 3) &&
					(((Node*)getNthElementOfList(output, 1))->id == 2) &&
					(((Node*)getNthElementOfList(output, 2))->id == 1) &&
					(((Node*)getNthElementOfList(output, 3))->id == 0) &&
					(((Node*)getNthElementOfList(output, 4))->id == 4)
			) || (
					(((Node*)getNthElementOfList(output, 0))->id == 3) &&
					(((Node*)getNthElementOfList(output, 1))->id == 1) &&
					(((Node*)getNthElementOfList(output, 2))->id == 2) &&
					(((Node*)getNthElementOfList(output, 3))->id == 4) &&
					(((Node*)getNthElementOfList(output, 4))->id == 0)
			) || (
					(((Node*)getNthElementOfList(output, 0))->id == 3) &&
					(((Node*)getNthElementOfList(output, 1))->id == 1) &&
					(((Node*)getNthElementOfList(output, 2))->id == 2) &&
					(((Node*)getNthElementOfList(output, 3))->id == 0) &&
					(((Node*)getNthElementOfList(output, 4))->id == 4)
			) || (
					(((Node*)getNthElementOfList(output, 0))->id == 3) &&
					(((Node*)getNthElementOfList(output, 1))->id == 2) &&
					(((Node*)getNthElementOfList(output, 2))->id == 4) &&
					(((Node*)getNthElementOfList(output, 3))->id == 1) &&
					(((Node*)getNthElementOfList(output, 4))->id == 0)
			) || (
					(((Node*)getNthElementOfList(output, 0))->id == 3) &&
					(((Node*)getNthElementOfList(output, 1))->id == 2) &&
					(((Node*)getNthElementOfList(output, 2))->id == 4) &&
					(((Node*)getNthElementOfList(output, 3))->id == 0) &&
					(((Node*)getNthElementOfList(output, 4))->id == 1)
			)
	);

	destroyList(output);
	destroyPredSuccGraph(g);
}

CuSuite* CuTopologicalOrderSuite() {
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, testCormen01);
	SUITE_ADD_TEST(suite, testCormen02);

	return suite;
}

