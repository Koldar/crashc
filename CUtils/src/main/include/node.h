/**
 * \file Node.h
 *
 * A node inside a graph
 *
 * Suppose you have a graph, like this one:
 *
 * @dot
 * digraph {
 * 	X;Y;Z
 *
 * 	X->Y;
 * 	Y->Z;
 * 	X->Z;
 * }
 * @enddot
 *
 * <tt>X,Y,Z</tt> are nodes and they are handled by this module. The main structure is ::Node.
 *
 *  Created on: Dec 22, 2016
 *      Author: koldar
 */

#ifndef NODE_H_
#define NODE_H_

#include <stdlib.h>
#include <stdbool.h>

#include "list.h"
#include "edge.h"
#include "utility.h"
#include "hashtable.h"

struct PredSuccGraph;

/**
 * The type of the ::Node::id
 */
typedef unsigned long NodeId;

typedef list NodeList;
typedef HT NodeHT;

/**
 * A single node within the graph
 *
 * This structure is used to represents the graph via a successors/predecessors notation, not via a adjacent matrix.
 *
 * Nodes may have a payload, aka some information natively associated to the graph vertex itself.
 * While it is not mandatory to have a payload (it might be NULL), having a payload can help exploit graph sturcture for your
 * need.
 *
 * You can access the payload with ::getNodePayloadAs.
 * If you have a payload, be sure to populate correctly the following global variables:
 * \li ::nodePayloadComparatorGlobal;
 * \li ::nodePayloadDestructorGlobal;
 * \li ::nodePayloadStringifyGlobal;
 * \li ::nodePayloadClonerGlobal;
 *
 * They are used inside the graph code during graph destruction, graph comparation and other stuff.
 * Failing to do will likely to generate unknown behaviour or memory leaks.
 */
typedef struct Node {
	///ID that uniquely identiies the node throughout all the graph
	NodeId id;
	/**
	 * Hash table containing all the edges starting from this node
	 *
	 * The key of the hashtable are the sink id of the edges while the values of the hashtables are the edges themselves
	 */
	EdgeHT* successors;
	/**
	 * the payload associated to this node. Can be NULL
	 */
	void* payload;
} Node;

/**
 * Create a new node
 *
 * @param[in] id the id of the node. Must be unique for every nodes
 */
Node* initNode(const NodeId id);

/**
 * Like ::initNode but it set also the payload
 *
 * @param[in] id the id of the node. Must be unique for every nodes
 * @param[in] payload the payload to associate to this node
 */
Node* initNodeWithPayload(const NodeId id, void* payload);

/**
 * like ::initNode but the id is assigned automatically
 */
Node* initNodeAuto();

/**
 * Destroy the node
 *
 * The function will remove the node and the edges of the node. Its successors and predecessors will be left untouched:
 * this <b>will lead to inconsistencies inside ::Node::successors and ::Node::predecessors structure!</b>
 *
 * \attention
 * Use this function only if you want to delete not only this node, but also its successors and predecessors!
 *
 * Furthermore, the function won't free from the memory the payload. If you want to remove the payload as well, you
 * should invoke ::destroyNodeWithPayload
 *
 * @param[in] n the node to remove from memory
 */
void destroyNode(Node* n);

/**
 * Destroy the node (payload included)
 *
 * The function will remove the node and the edges of the node. Its successors and predecessors will be left untouched:
 * this <b>will lead to inconsistencies inside ::Node::successors and ::Node::predecessors structure!</b>
 *
 * \attention
 * Use this function only if you want to delete not only this node, but also its successors and predecessors!
 *
 * @param[in] n the node to remove from memory
 * @param[in] nodePayloadDestructor the function to use to free the payload of the node from the memory
 * @param[in] edgePayloadDestructor the function to use to free the paylaod of the edges
 */
void destroyNodeWithPayload(Node* node, destructor nodePayloadDestructor, destructor edgePayloadDestructor);

/**
 * Add an edge in the successor list of the node
 *
 * \note if an edge is already present betwen the 2 nodes, it will be replaced
 *
 * @param[in] source the source of the edge
 * @param[in] sink the sink of the edge
 * @param[in] edgePayload the label we need to put on the edge to add
 * @return the edge just added
 */
Edge* addEdgeInNode(Node* restrict source, Node* restrict sink, void* edgePayload);

/**
 * Add an edge in the successor list of the node
 *
 * \note
 * if an edge is already present between the 2 nodes, it will be replaced
 *
 * @param[in] source the node that will contain the new edge
 * @param[in] e the edge to add
 * @return \c e itself;
 */
Edge* addEdgeDirectlyInNode(Node* source, const Edge* e);

/**
 * Removes an edge from the 2 nodes
 *
 * \post
 * 	\li the edge is removed from the memory as well
 *
 * @param[in] source the source of the edge
 * @param[in] sink the sink of the edge
 */
void removeEdgeInNode(Node* restrict source, Node* restrict sink);

/**
 * Find the edge between 2 node
 *
 * @param[in] source the source fo the edge
 * @param[in] sink the sink of the edge
 * @return
 * 	\li the edge between 2 nodes;
 * 	\li NULL if there is no edge between the 2 nodes;
 */
Edge* findEdgeInNode(const Node* restrict source, const Node* restrict sink);

/**
 * Get all the edges whose source is \c source
 *
 * \attention
 * do not edit this hashtable, becuase such changes will be made to the actual node!
 *
 * @param[in] source the node involved
 * @return a hashtable containing all the edges whose source is \c source
 */
EdgeHT* getEdgeHTOfNode(const Node* source);

/**
 * fetch the number of successors of the node \c source
 *
 * @param[in] source the node to analyze
 * @return the size of the output of ::getEdgeHTOfNode
 */
int getNumberOfEdgesInNode(const Node* source);

/**
 * Get all the edges whose source is \c source
 *
 * \note
 * You can add and remove edges in this list, but don't edit
 * the original edges, since you will alter the original structure!
 *
 * \attention be sure to destroy the list after the use via ::destroyList or ::destroyListWithElements
 *
 * @param[in] source the node involved
 * @return a list containing all the edges whose source is \c source
 */
EdgeList* getEdgeListOfNode(const Node* source);

/**
 * The has of a node
 *
 * @param[in] n the node involved
 * @return \c n hash code
 */
int getHashOfNode(const Node* n);

/**
 * Clone node
 *
 * \note
 * The payload will be copied by reference. If you want to copy the payload as well use ::cloneNodeWithPayload
 *
 * @param[in] n the node to clone
 * @return the cloned node
 */
Node* cloneNode(const Node* n);

/**
 * Clone node (payload included
 *
 *
 * @param[in] n the node to clone
 * @param[in] payloadCloner the function to use to clone the node payload
 * @return the cloned node
 */
Node* cloneNodeWithPayload(const Node* n, cloner payloadCloner);

/**
 * Compare 2 different nodes
 *
 * @param[in] a the first node to compare
 * @param[in] b the second node to compare
 * @param[in] payloadComparator the function to use to compare the nodes payloads
 * @return
 * 	\li true if the 2 nodes are the same;
 * 	\li false otherwise
 */
bool compareNodes(const Node* a, const Node* b, comparator payloadComparator);

Node* loadNodeFromFileWithoutEdges(FILE* f, object_deserializer nodeDeserializer);

void storeNodeInFileWithoutEdges(FILE* f, const Node* n, object_serializer nodeSerializer);

/**
 * Get the payload of the node already casted
 *
 * @param[in] node a point of ::Node
 * @param[in] type the type of the payload. example may be \c int, \c Foo
 */
#define getNodePayloadAs(node, type) ((type)node->payload)

#endif /* NODE_H_ */
