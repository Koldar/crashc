/*
 * topologicalOrder.c
 *
 *  Created on: May 31, 2017
 *      Author: koldar
 */


#include "topologicalOrder.h"
#include "log.h"

static void doCormenTopologicalOrder(PredSuccGraph* graph, NodeList* output);
static void doCormenTopologicalOrderRecursive(PredSuccGraph* graph, Node* n, NodeList* output);

void doStaticTopologicalOrderWith(to_impl implementation, PredSuccGraph* graph, NodeList* output) {
	switch (implementation) {
	case TO_CORMEN:
		doCormenTopologicalOrder(graph, output);
		return;
	}
}

//CORMEN

#define CORMEN_WHITE 0
#define CORMEN_GRAY 1
#define CORMEN_BLACK 2
static short int visited[CUTILS_ARRAY_SIZE];

/**
 * compute the topological order thank to Cormen's algorithm
 */
static void doCormenTopologicalOrder(PredSuccGraph* graph, NodeList* output) {

	//visited are all marked as 0
	memset(visited, CORMEN_WHITE, sizeof(short int) * CUTILS_ARRAY_SIZE);
	unsigned long whiteNodesLeft = getSizeOfGraph(graph);
	ITERATE_VALUES_ON_HT(&graph->nodes, node, Node) {
		if (visited[node->id] == CORMEN_WHITE) {
			doCormenTopologicalOrderRecursive(graph, node, output);
		}
	}
}


static void doCormenTopologicalOrderRecursive(PredSuccGraph* graph, Node* n, NodeList* output) {
	if (visited[n->id] == CORMEN_GRAY) {
		critical("we detected a cycle into the supposed DAG graph @ node %ld", n->id);
		ERROR_APPLICATION_FAILED("topological ordering", "Cormen", "graph", "graph");
	}

	if (visited[n->id] == CORMEN_WHITE) {
		visited[n->id] = CORMEN_GRAY;
		ITERATE_VALUES_ON_HT(&n->successors, edge, Edge) {
			doCormenTopologicalOrderRecursive(graph, edge->sink, output);
		}
		visited[n->id] = CORMEN_BLACK;
		addHeadInList(output, n);
	}

}

// END CORMEN
