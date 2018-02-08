/*
 * PredSuccGraph.c
 *
 *  Created on: Dec 22, 2016
 *      Author: koldar
 */


#include "predsuccgraph.h"

#include "node.h"
#include "log.h"
#include "utility.h"
#include "defaultFunctions.h"

static void computeDotFile(PredSuccGraph* graph, const char* fileName, NodeId highlightedNodeid);

PredSuccGraph* initPredSuccGraph() {
	PredSuccGraph* retVal = (PredSuccGraph*) malloc(sizeof(PredSuccGraph));
	if (retVal == NULL) {
		ERROR_MALLOC();
	}
	retVal->size = 0;
	retVal->nodes = initHT();

	retVal->nodePayloadCloner = defaultPayloadCloner;
	retVal->nodePayloadComparator = defaultPayloadComparator;
	retVal->nodePayloadDestructor = defaultPayloadDestructor;
	retVal->nodePayloadStringify = defaultPayloadStringify;
	retVal->nodePayloadColorer = defaultPayloadColor;
	retVal->nodePayloadSerializer = defaultSerializer;
	retVal->nodePayloadDeserializer = defaultDeserializer;

	retVal->edgePayloadCloner = defaultPayloadCloner;
	retVal->edgePayloadComparator = defaultPayloadComparator;
	retVal->edgePayloadDestructor = defaultPayloadDestructor;
	retVal->edgePayloadStringify = defaultPayloadStringify;
	retVal->edgePayloadColorer = defaultPayloadColor;
	retVal->edgePayloadSerializer = defaultSerializer;
	retVal->edgePayloadDeserializer = defaultDeserializer;
	return retVal;
}

static PredSuccGraph* contextDestroyNodesGraph = NULL;
static void lambdaDestroyNodes(Node* n) {
	destroyNodeWithPayload(n, contextDestroyNodesGraph->nodePayloadDestructor, contextDestroyNodesGraph->edgePayloadDestructor);
}

void destroyPredSuccGraph(PredSuccGraph* g) {
	contextDestroyNodesGraph = g;
	destroyHTWithElements(&g->nodes, lambdaDestroyNodes);
	free(g);
}

PredSuccGraph* clonePredSuccGraph(PredSuccGraph* graph) {
	PredSuccGraph* cloned = initPredSuccGraph();
	Node* source = NULL;
	Node* sink = NULL;

	cloned->nodePayloadCloner = graph->nodePayloadCloner;
	cloned->nodePayloadComparator = graph->nodePayloadComparator;
	cloned->nodePayloadDestructor = graph->nodePayloadDestructor;
	cloned->nodePayloadStringify = graph->nodePayloadStringify;
	cloned->nodePayloadColorer = graph->nodePayloadColorer;
	cloned->nodePayloadSerializer = graph->nodePayloadSerializer;
	cloned->nodePayloadDeserializer = graph->nodePayloadDeserializer;

	cloned->edgePayloadCloner = graph->edgePayloadCloner;
	cloned->edgePayloadComparator = graph->edgePayloadComparator;
	cloned->edgePayloadDestructor = graph->edgePayloadDestructor;
	cloned->edgePayloadStringify = graph->edgePayloadStringify;
	cloned->edgePayloadColorer = graph->edgePayloadColorer;
	cloned->edgePayloadSerializer = graph->edgePayloadSerializer;
	cloned->edgePayloadDeserializer = graph->edgePayloadDeserializer;

	debug("cloning nodes... size=%d", cloned->size);
	for (NodeId nodeId=0; nodeId<graph->size; nodeId++) {
		source = getNodeInGraph(graph, nodeId);
		if (source == NULL) {
			ERROR_NOT_SUCH_OBJECT("node", nodeId);
		}
		addNodeInGraph(cloned, initNodeWithPayload(nodeId, graph->nodePayloadCloner(source->payload)));
	}
	debug("done");

	EdgeList* edgeList = initList();

	//ok, now we clone the edges as well
	for (NodeId nodeId=0; nodeId<cloned->size; nodeId++) {
		source = getNodeInGraph(graph, nodeId);
		if (source == NULL) {
			ERROR_NOT_SUCH_OBJECT("node", nodeId);
		}

		ITERATE_VALUES_ON_HT(&source->successors, edge, Edge) {
			debug("adding edge %d->%d in the clone", source->id, edge->sink->id);
			addHeadInList(
					edgeList,
					initEdge(
							getNodeInGraph(cloned, source->id),
							getNodeInGraph(cloned, edge->sink->id),
							cloned->edgePayloadCloner(edge->payload)
					)
			);
		}

		ITERATE_ON_LIST(edgeList, cell, value, Edge) {
			addEdgeDirectlyInGraph(cloned, value);
		}
		clearList(edgeList);
	}
	destroyList(edgeList);
	return cloned;

}

void addNodeInGraph(PredSuccGraph* g, const Node* n) {
	addItemInHTWithKey(&g->nodes, n->id, n);
	g->size++;
}

Node* addNewNodeInGraph(PredSuccGraph* g, NodeId id, void* payload) {
	Node* n = initNodeWithPayload(id, payload);
	addNodeInGraph(g, n);
	return n;
}

Node* getNodeInGraph(const PredSuccGraph* g, const NodeId id) {
	debug("node id is %ld", id);
	return getItemInHT(&g->nodes, id);
}

int getNumberOfEdgesOfNodeInGraph(const PredSuccGraph* g, NodeId id) {
	Node* n = getNodeInGraph(g, id);
	if (n == NULL) {
		ERROR_NOT_SUCH_OBJECT("node", id);
	}
	return getNumberOfEdgesInNode(n);
}

Edge* addEdgeDirectlyInGraph(PredSuccGraph* g, const Edge* edge) {
	Node* source = getNodeInGraph(g, edge->source->id);
	if (source == NULL) {
		ERROR_NOT_SUCH_OBJECT("node", edge->source->id);
	}
	addEdgeDirectlyInNode(source, edge);
	return edge;
}

Edge* addEdgeInGraph(Node* restrict source, Node* restrict sink, void* edgePayload) {
	return addEdgeInNode(source, sink, edgePayload);
}

Edge* addEdgeInGraphById(PredSuccGraph* graph, NodeId sourceId, NodeId sinkId, void* edgePayload) {
	Node* source = getNodeInGraph(graph, sourceId);
	if (source == NULL) {
		ERROR_NOT_SUCH_OBJECT("source", sourceId);
	}
	Node* sink = getNodeInGraph(graph, sinkId);
	if (sink == NULL) {
		ERROR_NOT_SUCH_OBJECT("sink", sinkId);
	}
	return addEdgeInGraph(source, sink, edgePayload);
}

void removeEdgeInGraph(Node* restrict source, Node* restrict sink) {
	removeEdgeInNode(source, sink);
}

void removeEdgeInGraphById(PredSuccGraph* graph, const NodeId sourceId, const NodeId sinkId, const bool removeFlipped) {
	Node* source = getNodeInGraph(graph, sourceId);
	Node* sink = getNodeInGraph(graph, sinkId);
	if (source == NULL) {
		ERROR_NOT_SUCH_OBJECT("source", sourceId);
	}
	if (sink == NULL) {
		ERROR_NOT_SUCH_OBJECT("sink", sinkId);
	}
	removeEdgeInGraph(source, sink);

	if (removeFlipped) {
		removeEdgeInGraph(sink, source);
	}
}

Edge* getEdgeInGraph(const Node* restrict source, const Node* restrict sink) {
	return findEdgeInNode(source, sink);
}

Edge* getEdgeInGraphById(const PredSuccGraph* graph, const NodeId sourceId, const NodeId sinkId) {
	Node* source = NULL;
	Node* sink = NULL;
	source = getNodeInGraph(graph, sourceId);
	sink = getNodeInGraph(graph, sinkId);
	if (source == NULL || sink == NULL) {
		return NULL;
	}
	return findEdgeInNode(source, sink);
}

bool containsEdgeInGraphById(const PredSuccGraph* graph, NodeId sourceId, NodeId sinkId) {
	return getEdgeInGraphById(graph, sourceId, sinkId) != NULL;
}

//TODO the name should be a property of the graph
void printGraph(PredSuccGraph* graph, const char* fileName) {
	printGraphWithOptions(graph, DC_CIRCO, fileName, NULL, -1);
}

void printGraphWithAdditionalName(PredSuccGraph* graph, const char* fileName, const char* additionalName) {
	printGraphWithOptions(graph, DC_CIRCO, fileName, additionalName, -1);
}

void printGraphWithOptions(const PredSuccGraph* graph, dot_commands cmd, const char* fileName, const char* additionalName, NodeId highlightedNode) {
	//FIXME do not crash if circo is not available on the system
	char nameBuffer[BUFFER_SIZE];
	info("additional name is %s", additionalName);
	sprintf(nameBuffer, "%s_%s", fileName, (additionalName == NULL) ? "" : additionalName);

	computeDotFile(graph, nameBuffer, highlightedNode);

	char cmdName[BUFFER_SIZE];
	switch (cmd) {
	case DC_DOT: { sprintf(cmdName, "dot"); break; }
	case DC_CIRCO: { sprintf(cmdName, "circo"); break; }
	case DC_NEATO: { sprintf(cmdName, "neato"); break; }
	case DC_SFDP: { sprintf(cmdName, "sfdp"); break; }
	default: {
		ERROR_UNHANDLED_CASE("dot command", cmd);
	}
	}

	char buffer[BUFFER_SIZE];
	sprintf(buffer, "%s -Tsvg %s.dot -o %s.svg", cmdName, nameBuffer, nameBuffer);
	system(buffer);
	//removes the dotfile, if exists
	sprintf(buffer, "%s.dot", nameBuffer);
	//try to delete dot files. If removes fails we ignore the return value
	remove(buffer);
}

static void computeDotFile(PredSuccGraph* graph, const char* fileName, NodeId highlightedNodeid) {
	char fileNameWithExtension[BUFFER_SIZE];
	sprintf(fileNameWithExtension, "%s.dot", fileName);

	FILE* fout = fopen(fileNameWithExtension, "w");
	Node* tmp;
	int tab = 0;
	EdgeList* successors;
	char* payloadStr;
	char str[BUFFER_SIZE];

	if (fout == NULL) {
		ERROR_FILE(fileName);
	}

	fprintfTabbed(fout, tab, "digraph {");
	tab += 1;
	for (NodeId i=0; i<getSizeOfGraph(graph); i++) {
		Node* n = getNodeInGraph(graph, i);
		if (n == NULL) {
			ERROR_NOT_SUCH_OBJECT("node", i);
		}
		str[0] = '\0';
		payloadStr = NULL;
		payloadStr = graph->nodePayloadStringify(n->payload);
		if (payloadStr != NULL) {
			strcat(str, "\\n");
			strcat(str, payloadStr);
		}
		if (i != highlightedNodeid) {
			fprintfTabbed(fout, tab, "N%04d [label=\"%d%s\"];\n", i, i, str);
		} else {
			fprintfTabbed(fout, tab, "N%04d [label=\"%d%s\" style=\"filled\" fillcolor=\"blue\"];\n", i, i, str);
		}
		if (payloadStr != NULL) {
			free(payloadStr);
		}
	}

	for (int i=0; i<getSizeOfGraph(graph); i++) {
		tmp = getNodeInGraph(graph, i);


		ITERATE_VALUES_ON_HT(&tmp->successors, edge, Edge) {
			char* str = graph->edgePayloadStringify(edge->payload);
			fprintfTabbed(fout, tab, "N%04d -> N%04d [label=\"%s\" color=\"#%06d\"];\n",
					tmp->id,
					edge->sink->id,
					str,
					graph->edgePayloadColorer(edge->payload)
			);
			free(str);
		}


		//		TODO REMOVE successors = tmp->successors;
		//		while (successors != NULL) {
		//			fprintfTabbed(fout, tab, "N%04d -> N%04d [label=\"%s\" color=\"%s\"];\n",
		//					tmp->id,
		//					((Edge*)successors->pointer)->sink->id,
		//					getStringFromEdgeLabel(((Edge*)successors->pointer)->label),
		//					isEdgeFlawed(((Edge*)successors->pointer)) ? "red" : "black"
		//			);
		//			successors = successors->next;
		//		}
		fprintfTabbed(fout, tab, "\n");
	}
	fprintfTabbed(fout, tab, "}");

	fflush(fout);
	fclose(fout);
}

int getSizeOfGraph(const PredSuccGraph* graph) {
	return graph->size;
}

EdgeList* getEdgeListOfPredSuccGraph(const PredSuccGraph* graph) {
	EdgeList* retVal = (EdgeList*) initList();
	Node* source = NULL;
	EdgeList* sourceSuccessors = NULL;
	Edge* fe;

	for (int sourceId=0; sourceId<graph->size;sourceId++ ) {
		source = getNodeInGraph(graph, sourceId);
		if (source == NULL) {
			ERROR_NOT_SUCH_OBJECT("source", sourceId);
		}

		ITERATE_VALUES_ON_HT(&source->successors, edge, Edge) {
			addHeadInList(retVal, edge);
		}

		//		todo sourceSuccessors = getEdgeListOfNode(source);
		//		while (sourceSuccessors != NULL) {
		//			retVal = addHead(retVal, (Edge*)sourceSuccessors->pointer);
		//			sourceSuccessors = sourceSuccessors->next;
		//		}
	}

	return retVal;
}

int getEdgesSize(const PredSuccGraph* graph) {
	int retVal = 0;
	Node* source = NULL;
	EdgeList* sourceSuccessors = NULL;
	Edge* fe;

	for (int sourceId=0; sourceId<graph->size;sourceId++ ) {
		source = getNodeInGraph(graph, sourceId);
		if (source == NULL) {
			ERROR_NOT_SUCH_OBJECT("source", sourceId);
		}

		retVal += getSizeOfHT(&source->successors);

		//		todo sourceSuccessors = getEdgeListOfNode(source);
		//		while (sourceSuccessors != NULL) {
		//			retVal++;
		//			sourceSuccessors = sourceSuccessors->next;
		//		}
	}

	return retVal;
}

static comparator contextFindEdgeByOtherComparator;
static Edge* contextFindEdgeByOther;
static bool lambdaFindEdgeByOther(Edge* e) {
	return getEdgeCmpWithPayload(e, contextFindEdgeByOther, contextFindEdgeByOtherComparator);
}

bool predSuccGraphCmp(const PredSuccGraph* restrict g1, const PredSuccGraph* restrict g2) {
	bool retVal = false;

	info("g1 %d g2 %d", getSizeOfGraph(g1), getSizeOfGraph(g2));
	if (getSizeOfGraph(g1) != getSizeOfGraph(g2)) {
		return false;
	}

	EdgeHT* edgeHT1 = NULL;
	EdgeHT* edgeHT2 = NULL;
	Edge* e1 = NULL;
	Edge* e2 = NULL;
	Node* n1 = NULL;
	Node* n2 = NULL;

	for (NodeId id=0; id < getSizeOfGraph(g1); id++) {
		n1 = getNodeInGraph(g1, id);
		n2 = getNodeInGraph(g2, id);

		edgeHT1 = getEdgeHTOfNode(n1);
		edgeHT2 = getEdgeHTOfNode(n2);

		if (getSizeOfHT(&edgeHT1) != getSizeOfHT(&edgeHT2)) {
			return false;
		}
		info("g1 comaprator is %p while g2 comaprator is %p", g1->edgePayloadComparator, g2->edgePayloadComparator);
		if (g1->edgePayloadComparator != g2->edgePayloadComparator) {
			ERROR_EQUALITY_FAILED("graph edge payload comparator", "g1", "graph edge payload comparator", "g2");
		}
		ITERATE_VALUES_ON_HT(&edgeHT1, edge, Edge) {
			contextFindEdgeByOther = edge;
			contextFindEdgeByOtherComparator = g1->edgePayloadComparator;
			if (!isHTContainingItems(&edgeHT2, lambdaFindEdgeByOther)) {
				info("edge %d -> %d is not present inside hashtable 2", edge->source->id, edge->sink->id);
				return false;
			}
		}


		//		while (edgeHT1 != NULL) {
		//			e1 = (Edge*)edgeHT1->pointer;
		//			contextFindEdgeBySink = e1->sink;
		//			e2 = find(edgeHT2, lambdaFindEdgeBySink);
		//			if (e2 == NULL) {
		//				return false;
		//			}
		//			if (e1->label != e2->label) {
		//				return false;
		//			}
		//			edgeHT1 = edgeHT1->next;
		//		}
	}

	return true;
}

//static object_serializer lambdaStoreNodeInFileContext_nodeSerializer = NULL;
//static object_serializer lambdaStoreNodeInFileContext_edgeSerializer = NULL;
//static int lambdaStoreNodeInFile(FILE* f, void* n) {
//	storeNodeInFileWithoutEdges(f, n, lambdaStoreNodeInFileContext_nodeSerializer);
//	//storeNodeInFile(f, n, lambdaStoreNodeInFileContext_nodeSerializer, lambdaStoreNodeInFileContext_edgeSerializer);
//	return 0;
//}
//
//static object_deserializer lambdaLoadNodeFromFileContext_nodeDeserializer = NULL;
//static object_deserializer lambdaLoadNodeFromFileContext_edgeDeserializer = NULL;
//static PredSuccGraph* lambdaLoadNodeFromFileContext_graph = NULL;
//static int lambdaLoadNodeFromFile(FILE* f, void** n) {
//	*n = loadNodeFromFile(f, lambdaLoadNodeFromFileContext_graph, lambdaLoadNodeFromFileContext_nodeDeserializer, lambdaLoadNodeFromFileContext_edgeDeserializer);
//	return 0;
//}

static object_serializer lambdaStoreEdgeInFileContext_edgeSerializer = NULL;
static int lambdaStoreEdgeInFile(FILE* f, Edge* e) {
	storeEdgeInFile(f, e, lambdaStoreEdgeInFileContext_edgeSerializer);
	return 0;
}


static object_deserializer lambdaLoadEdgeFromFileContext_edgeDeserializer = NULL;
static PredSuccGraph* lambdaLoadEdgeFromFileContext_graph = NULL;
static int lambdaLoadEdgeFromFile(FILE* f, Edge** e) {
	*e = loadEdgeFromFile(f, lambdaLoadEdgeFromFileContext_graph, lambdaLoadEdgeFromFileContext_edgeDeserializer);
	return 0;
}

void storeGraphInFile(FILE* f, const PredSuccGraph* g) {
	//size
	fwrite(&g->size, sizeof(g->size), 1, f);
	//we store the elements
	ITERATE_VALUES_ON_HT(&g->nodes, n, Node) {
		storeNodeInFileWithoutEdges(f, n, g->nodePayloadSerializer);
	}
	//now we store the edges
	int count;
	ITERATE_VALUES_ON_HT(&g->nodes, n2, Node) {
		fwrite(&n2->id, sizeof(n->id), 1, f);
		lambdaStoreEdgeInFileContext_edgeSerializer = g->edgePayloadSerializer;
		storeHTInFile(f, &n2->successors, lambdaStoreEdgeInFile);
	}
}

PredSuccGraph* loadGraphFromFile(FILE* f, object_deserializer nodeDeserializer, object_deserializer edgeDeserializer) {
	Node* n;
	int size;

	PredSuccGraph* retVal = initPredSuccGraph();
	retVal->nodePayloadDeserializer = nodeDeserializer;
	retVal->edgePayloadDeserializer = edgeDeserializer;
	//size
	fread(&size, sizeof(retVal->size), 1, f);
	//elements
	for (int i=0; i<size; i++) {
		n = loadNodeFromFileWithoutEdges(f, nodeDeserializer);
		if (n == NULL) {
			ERROR_NOT_SUCH_OBJECT("node", i);
		}
		destroyHT(&n->successors);
		addNodeInGraph(retVal, n);
	}
	//now we load the edge
	NodeId id;
	ITERATE_VALUES_ON_HT(&retVal->nodes, n2, Node) {
		//the id of the node to read
		fread(&id, sizeof(id), 1, f);
		lambdaLoadEdgeFromFileContext_edgeDeserializer = edgeDeserializer;
		lambdaLoadEdgeFromFileContext_graph = retVal;
		n2->successors = loadHTFromFile(f, lambdaLoadEdgeFromFile);
	}
	return retVal;
}
