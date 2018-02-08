/*
 * simple_loop_computer.c
 *
 *  Created on: Jul 25, 2017
 *      Author: koldar
 */


#include "simple_loops_computer.h"
#include <stdbool.h>
#include "scc.h"
#include "heap.h"
#include "stack.h"
#include "dynamic_array.h"
#include "log.h"

typedef HT node_list_ht;
typedef dynamic_1D_array bool_array;
typedef Stack node_stack;

typedef struct johnson_support {
	/**
	 * array of booleans.
	 *
	 * A cell is true iff the node is inside the building loop.
	 * False otherwise
	 */
	bool_array* blocked;
	/**
	 * An hashtable representing B
	 *
	 * In B where each key is an id of a node in ::johnson_support::graph while
	 * the value is a list of nodes in ::johnson_support::graph.
	 */
	node_list_ht* B;
	/**
	 * A stack where to put the vertex on ::circuit
	 */
	node_stack* stack;
	/**
	 * The function used to check if an edge can be traversed or not
	 */
	edge_traverser t;
	/**
	 * an hashtable where containing each node that we need to consider inside the SCC algorithm computation.
	 *
	 * If we need to consider a node, we need to have an entry in this field indexed by the **node id**.
	 */
	bool_ht* included;
} johnson_support;

static void unblock(bool_array* blocked, node_list_ht* b, NodeId u);
static bool circuit(const PredSuccGraph* graph, const scc_graph* sccGraph, const scc* sccOfRoot, node_stack* stack, bool_array* blocked, node_list_ht* b, NodeId rootId, NodeId vId, loop_list* retVal);
static void cricuit_finding_algorithm(johnson_support* support, const PredSuccGraph* graph, loop_list* retVal);

johnson_support* initJohnsonSupport() {
	johnson_support* retVal = malloc(sizeof (johnson_support));
	if (retVal == NULL) {
		ERROR_MALLOC();
	}

	retVal->B = initHT();
	retVal->blocked = NULL;
	retVal->stack = NULL;
	retVal->t = NULL;
	retVal->included = initHT();

	return retVal;
}

void destroyJohnsonSupport(johnson_support* support) {
	destroyDynamicArray(support->blocked);
	destroyHTWithElements(&support->B, destroyList);
	destroyStack(support->stack);
	destroyHT(&support->included);
}

loop_list* computeSimpleLoopsInGraph(johnson_support* support, const PredSuccGraph* g, edge_traverser t) {
	loop_list* retVal = initList();

	// ***************** SETUP SUPPORT ********************
	if (support->blocked != NULL && getSizeOfDynamicArray(support->blocked) < getSizeOfGraph(g)) {
		destroyDynamicArray(support->blocked);
	}
	support->blocked = initDynamicArray(bool, getSizeOfGraph(g));

	if (support->stack != NULL && getStackMaxSize(support->stack) < getSizeOfGraph(g)) {
		destroyStack(support->stack);
	}
	support->stack = initStack(getSizeOfGraph(g));
	support->t = t;

	clearHT(&support->included);
	clearStack(support->stack);
	clearDynamicArrayTo(support->blocked, bool, true);
	ITERATE_ON_HT(&support->B, el) {
		destroyList((node_list*)el->data);
		free(el);
	}

	// ****************** EXECUTE ALGORITHM *******************

	cricuit_finding_algorithm(support, g, retVal);

	return retVal;
}

static void cricuit_finding_algorithm(johnson_support* support, const PredSuccGraph* graph, loop_list* retVal) {
	scc_graph* sccGraph = NULL;
	scc* sccAnalyzed;

	for (NodeId id=0; id<getSizeOfGraph(graph); id++) {
		addItemInHTWithKey(&support->included, id, true);
	}

	//loop over the nodes
	for (NodeId s=0; s<getSizeOfGraph(graph); s++) {
		sccGraph = initStronglyConnectedComponentsGraph(graph, support->t, false, support->included);
		//scc containing s
		sccAnalyzed = getStrongConnectedComponentOfNodeById(sccGraph, s);
		if (sccAnalyzed == NULL) {
			break;
		}
		Node* root = getNodeWithMinimumIdInStrongConnectedComponent(sccAnalyzed);

		ITERATE_THROUGH_HEAP(getNodesInsideStrongConnectedComponent(sccAnalyzed), node, Node*) {
			setDynamicArrayCellTo(support->blocked, node->id, bool, false);
			NodeList* b_node = getItemInHT(&support->B, node->id);
			if (b_node == NULL) {
				addItemInHTWithKey(&support->B, node->id, initList());
			} else {
				clearList(b_node);
			}
		}

		circuit(graph, sccGraph, sccAnalyzed, support->stack, support->blocked, support->B, root->id, root->id, retVal);

		//we now remove the node "s" from the graph, since we have computed all the loop involving it
		if (sccGraph != NULL) {
			destroyStronglyConnectedComponentsGraph(sccGraph);
		}
		deleteItemInHT(&support->included, s);
	}
}

static void unblock(bool_array* blocked, node_list_ht* b, NodeId u) {
	//put the node outside the building loop
	setDynamicArrayCellTo(blocked, u, bool, false);

	NodeList* bu = getItemInHT(&b, u);
	VARIABLE_ITERATE_ON_LIST(bu, prevCell, cell, w, Node) {
		removeElementInListCell(bu, &prevCell, cell);
		if (getDynamicArrayCellValue(blocked, w->id, bool)) {
			unblock(blocked, b, w->id);
		}
	}
}

static bool circuit(const PredSuccGraph* graph, const scc_graph* sccGraph, const scc* sccOfRoot, node_stack* stack, bool_array* blocked, node_list_ht* b, NodeId rootId, NodeId vId, loop_list* retVal) {
	bool f;
	Node* v;
	Node* w;

	f = false;
	v = getNodeInGraph(graph, vId);
	pushToStack(stack, v);
	setDynamicArrayCellTo(blocked, vId, bool, true);

	// ******************** L1 ***************************

	ITERATE_VALUES_ON_HT(&v->successors, e, Edge) {
		w = e->sink;

		if (!isNodeInsideStrongConnectedComponentById(sccGraph, w->id, sccOfRoot->id)) {
			//the successor is in another scc. We ignore it since it can't lead to cycles
			continue;
		}

		if (w->id == rootId) {
			//output circuit composed of stack followed by s
			loop* l = initList();
			ITERATE_THROUGH_STACK(stack, n, Node*) {
				addTailInList(l, n);
			}
			addTailInList(retVal, l);

			f = true;
		} else if (!getDynamicArrayCellValue(blocked, w->id, bool)) {
			if (circuit(graph, sccGraph, sccOfRoot, stack, blocked, b, rootId, w->id, retVal)) {
				f = true;
			}
 		}
	}

	// ******************** L2 ************************

	if (f) {
		unblock(blocked, b, vId);
	} else {
		bool found;
		ITERATE_VALUES_ON_HT(&v->successors, e, Edge) {
			w = e->sink;
			NodeId wId = w->id;
			if (!isNodeInsideStrongConnectedComponentById(sccGraph, w->id, sccOfRoot->id)) {
				//the successor is in another scc. We ignore it since it can't lead to cycles
				continue;
			}

			NodeList* bw = getItemInHT(&b, wId);
			if (bw == NULL) {
				bw = initList();
				addItemInHTWithKey(&b, wId, bw);
			}

			//search in bw for v
			found = false;
			ITERATE_ON_LIST(bw, cell, n, Node) {
				if (n->id == vId) {
					found = true;
					break;
				}
			}

			if (!found) {
				addTailInList(bw, v);
			}

		}
	}

	Node* expectedV = popFromStack(stack);
#ifdef DEBUG
	if (expectedV->id != v->id) {
		ERROR_UNHANDLED_CASE("popped from stack", expectedV->id);
	}
#endif

	return f;
}
