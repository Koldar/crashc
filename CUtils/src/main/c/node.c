/*
 * Node.c
 *
 *  Created on: Dec 22, 2016
 *      Author: koldar
 */

#include "node.h"
#include "log.h"
#include "edge.h"
#include "defaultFunctions.h"

static NodeId autoId = 0;


Node* initNode(const NodeId id) {
	return initNodeWithPayload(id, NULL);
}

Node* initNodeWithPayload(const NodeId id, void* payload) {
	Node* retVal = (Node*) malloc(sizeof(Node));
	if (retVal == NULL) {
		ERROR_MALLOC();
	}

	retVal->id = id;
	retVal->payload = payload;
	retVal->successors = initHT();

	return retVal;
}

Node* initNodeAuto() {
	return initNode(autoId++);
}

void destroyNode(Node* node) {
	destroyHTWithElements(&node->successors, destroyEdgeWithPayload);
	free(node);
}

static destructor contextDestroyEdgePaylaodDestructor = NULL;
static void lambdaDestroyEdge(Edge* e) {
	destroyEdgeWithPayload(e, contextDestroyEdgePaylaodDestructor);
}

void destroyNodeWithPayload(Node* node, destructor nodePayloadDestructor, destructor edgePayloadDestructor) {
	debug("destroying node %ld", node->id);
	contextDestroyEdgePaylaodDestructor = edgePayloadDestructor;
	destroyHTWithElements(&node->successors, lambdaDestroyEdge);
	if (node->payload != NULL) {
		nodePayloadDestructor(node->payload);
	}
	free(node);
}

EdgeList* getEdgeListOfNode(const Node* source) {
	EdgeList* retVal = initList();
	ITERATE_VALUES_ON_HT(&source->successors, edge, Edge) {
		debug("adding edge %d->%d to list", edge->source->id, edge->sink->id);
		addHeadInList(retVal, edge);
	}
	return retVal;
}

Edge* addEdgeInNode(Node* restrict source, Node* restrict sink, void* edgePayload) {
	Edge* edge = initEdge(source, sink, edgePayload);

	debug("find previous edge to remove: %d -> %d %p", source->id, sink->id, edgePayload);
	addEdgeDirectlyInNode(source, edge);
	return edge;
}

Edge* addEdgeDirectlyInNode(Node* source, const Edge* e) {
	deleteItemInHTWithElement(&source->successors, e->sink->id, destroyEdge);
	addItemInHTWithKey(&source->successors, e->sink->id, e);
	return e;
}

void removeEdgeInNode(Node* restrict source, Node* restrict sink) {
	deleteItemInHTWithElement(&source->successors, sink->id, destroyEdge);
}

Edge* findEdgeInNode(const Node* restrict source, const Node* restrict sink) {
	return getItemInHT(&source->successors, sink->id);
}

EdgeHT* getEdgeHTOfNode(const Node* source) {
	return source->successors;
}

int getNumberOfEdgesInNode(const Node* source) {
	return getSizeOfHT(&source->successors);
}


int getHashOfNode(const Node* n) {
	return n;
}



Node* cloneNode(const Node* n) {
	return initNodeWithPayload(n->id, n->payload);
}

Node* cloneNodeWithPayload(const Node* n, cloner payloadCloner) {
	return initNodeWithPayload(n->id, payloadCloner(n->payload));
}

bool compareNodes(const Node* a, const Node* b, comparator payloadComparator) {
	if (a->id != a->id) {
		return false;
	}
	return payloadComparator(a->payload, b->payload);
}

//static object_serializer lambdaStoreEdgeInFileContext = NULL;
//static int lambdaStoreEdgeInFile(FILE* f, void* e) {
//	storeEdgeInFile(f, e, lambdaStoreEdgeInFileContext);
//	return 0;
//}
//
//static struct PredSuccGraph* lamdaLoadNodeFromFileContext_graph = NULL;
//static object_deserializer lambdaLoadNodeFromFileContext_nodeDeserializer = NULL;
//static object_deserializer lambdaLoadNodeFromFileContext_edgeDeserializer = NULL;
//static int lambdaLoadNodeFromFile(FILE* f, void** p) {
//	Edge* other = loadEdgeFromFile(f, lamdaLoadNodeFromFileContext_graph, lambdaLoadNodeFromFileContext_nodeDeserializer, lambdaLoadNodeFromFileContext_edgeDeserializer);
//	Edge* inGraph = findEdgeInNode(other->source->id, other->sink->id);
//	//we create a new edge only if it is not inside the graph yet.
//	if (inGraph != NULL) {
//		destroyEdge(other);
//		*p = inGraph;
//	} else {
//		*p = other;
//	}
//	return 0;
//}
//
//void storeNodeInFile(FILE* f, const Node* n, object_serializer nodeSerializer, object_serializer edgeSerializer) {
//	char buffer[BUFFER_SIZE];
//
//	//id
//	fwrite(&n->id, sizeof(n->id), 1, f);
//	//payload
//	nodeSerializer(f, n->payload);
//	//successors
//	lambdaStoreEdgeInFileContext = edgeSerializer;
//	storeHTInFile(f, &n->successors, lambdaStoreEdgeInFile);
//}

void storeNodeInFileWithoutEdges(FILE* f, const Node* n, object_serializer nodeSerializer) {
	char buffer[BUFFER_SIZE];

	//id
	fwrite(&n->id, sizeof(n->id), 1, f);
	//payload
	nodeSerializer(f, n->payload);
}

Node* loadNodeFromFileWithoutEdges(FILE* f, object_deserializer nodeDeserializer) {
	Node* n;
	NodeId id;
	void* payload;
	//id
	fread(&id, sizeof(id), 1, f);
	//payload
	nodeDeserializer(f, &payload);
	n = initNodeWithPayload(id, payload);
	return n;
}

//Node* loadNodeFromFile(FILE* f, struct PredSuccGraph* g, object_deserializer nodeDeserializer, object_deserializer edgeDeserializer) {
//	Node* n;
//	NodeId id;
//	void* payload;
//	//id
//	fread(&id, sizeof(id), 1, f);
//	//payload
//	nodeDeserializer(f, &payload);
//	//successors
//	n = initNodeWithPayload(id, payload);
//	lamdaLoadNodeFromFileContext_graph = g;
//	lambdaLoadNodeFromFileContext_nodeDeserializer = nodeDeserializer;
//	lambdaLoadNodeFromFileContext_edgeDeserializer = edgeDeserializer;
//	n->successors = loadHTFromFile(f, lambdaLoadNodeFromFile);
//	return n;
//}
