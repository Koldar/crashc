/*
 * Edge.c
 *
 *  Created on: Dec 22, 2016
 *      Author: koldar
 */

# define _GNU_SOURCE
#include <stdio.h>
#include "node.h"
#include "log.h"
#include "utility.h"
#include <stdlib.h>
#include "edge.h"
#include "macros.h"
#include "defaultFunctions.h"

Edge* initEdge(const struct Node* restrict source, const struct Node* restrict sink, const void* payload) {
	Edge* retVal = (Edge*) malloc(sizeof(Edge));
	if (retVal == NULL) {
		ERROR_MALLOC();
	}

	retVal->source = source;
	retVal->sink = sink;
	retVal->payload = payload;

	return retVal;
}

Edge* cloneEdge(const Edge* e) {
	return initEdge(e->source, e->sink, e->payload);
}

Edge* cloneEdgeWithPayload(const Edge* e, cloner payloadCloner) {
	return initEdge(e->source, e->sink, payloadCloner(e->payload));
}

void destroyEdge(Edge* e) {
	free(e);
}

void destroyEdgeWithPayload(Edge* e, destructor payloadDestructor) {
	if (e->payload != NULL) {
		payloadDestructor(e->payload);
	}
	destroyEdge(e);
}

int getHashOfEdge(const Edge* e) {
	return e;
}

bool getEdgeCmp(const Edge* e1, const Edge* e2) {
	if ((e1 == NULL) ^ (e2 == NULL)) {
		return false;
	}
	return (e1->source->id == e2->source->id)&&(e1->sink->id == e2->sink->id);
}

bool getEdgeCmpWithPayload(const Edge* e1, const Edge* e2, comparator payloadComparator) {
	if (!getEdgeCmp(e1, e2)) {
		return false;
	}
	if (e1->payload == NULL ^ e2->payload == NULL) {
		return true;
	}
	return payloadComparator(e1->payload, e2->payload);
}

const char* getStringOfEdge(const Edge* e, stringify payloadStringigy) {
	char* retVal;
	char* payloadStr = NULL;
	if (e->payload != NULL) {
		payloadStr = payloadStringigy(e->payload);
	}
	asprintf(&retVal, "%d -> %d [label=\"%s\"]", e->source->id, e->sink->id, payloadStr);
	if (payloadStr != NULL) {
		free(payloadStr);
	}
	return retVal;
}

int getBufferStringOfEdge(const Edge* e, char* buffer) {
	int i = 0;

	i += sprintf(&buffer[i], "%ld->%ld", e->source->id, e->sink->id);

	return i;
}

void storeEdgeInFile(FILE* f, const Edge* e, object_serializer payload) {
	fwrite(&e->source->id, sizeof(NodeId), 1, f);
	fwrite(&e->sink->id, sizeof(NodeId), 1, f);
	payload(f, e->payload);
}

Edge* loadEdgeFromFile(FILE* f, struct PredSuccGraph* g, object_deserializer edgeDeserializer) {
	NodeId id;
	Node* source;
	Node* sink;
	void* payload;

	Node* getNodeInGraph(struct PredSuccGraph* g, NodeId id);

	fread(&id, sizeof(id), 1, f);
	source = getNodeInGraph(g, id);
	fread(&id, sizeof(id), 1, f);
	sink = getNodeInGraph(g, id);
	edgeDeserializer(f, &payload);
	return initEdge(source, sink, payload);
}
