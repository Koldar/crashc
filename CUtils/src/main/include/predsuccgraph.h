/**
 * \file PredSuccGraph.h
 *
 *  Created on: Dec 22, 2016
 *      Author: koldar
 */

#ifndef PREDSUCCGRAPH_H_
#define PREDSUCCGRAPH_H_

#include "utility.h"
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include "typedefs.h"

#include "node.h"
#include "edge.h"

/**
 * A graph containing nodes. Edges are retrieved observing a node configuring as source
 */
typedef struct PredSuccGraph {
	/**
	 * An hastable containing all the nodes inside the graph.
	 *
	 * The keys of the hastables are the IDs of the nodes whilst the values inside the ahstables
	 * are the nodes themselves.
	 *
	 * The data inside this array has a meaning only till the ::PredSuccGraph::size -th cell
	 */
	NodeHT* nodes;
	///the actual size of the graph
	unsigned int size;
	/**
	 * The function to use to compare the payload of 2 nodes
	 *
	 */
	comparator nodePayloadComparator;
	/**
	 * The function to use to destroy the layload of an node
	 *
	 */
	destructor nodePayloadDestructor;
	/**
	 * The function to use to compute the string representation of the paylaod of an node
	 *
	 */
	stringify nodePayloadStringify;
	/**
	 * The function to use to clone the payload of a node
	 */
	cloner nodePayloadCloner;
	/**
	 * The function to use when in images we need to choose a color for the node
	 */
	colorer nodePayloadColorer;
	object_serializer nodePayloadSerializer;
	object_deserializer nodePayloadDeserializer;
	/**
	 * The function to use to compare the payload of 2 edges
	 *
	 */
	comparator edgePayloadComparator;
	/**
	 * The function to use to destroy the layload of an edge
	 *
	 */
	destructor edgePayloadDestructor;
	/**
	 * The function to use to compute the string representation of the paylaod of an edge
	 *
	 */
	stringify edgePayloadStringify;
	buffer_string edgePayloadBufferStringify;
	/**
	 * The function to use to clone the payload of an edge
	 *
	 */
	cloner edgePayloadCloner;
	/**
	 * The function to use when in images we need to choose a color for the edge
	 */
	colorer edgePayloadColorer;
	object_serializer edgePayloadSerializer;
	object_deserializer edgePayloadDeserializer;

} PredSuccGraph;

/**
 * Initialize a new graph with no edges and not vertexes
 */
PredSuccGraph* initPredSuccGraph();

/**
 * Completely copy the whole graph
 *
 * \attention ensure to call ::destroyPredSuccGraph after using the return value of the function to avoid memory leaks
 *
 * @param[in] graph the graph to copy
 * @return a clone of the graph. Nodes, Edges wil be copied as well
 */
PredSuccGraph* clonePredSuccGraph(PredSuccGraph* graph);

/**
 * Destroy the whole graph
 *
 * \attention all the ::Node instances and the ::Edge instance of the graph will be removed as well!
 *
 * @param[in] g the graph to dispose
 */
void destroyPredSuccGraph(PredSuccGraph* g);

/**
 * Adds a node within the graph
 *
 * \pre
 * 	\li the node has unique identifier set to ::PredSuccGraph::size;
 * \post
 * 	\li ::PredSuccGraph::size increased by one;
 *
 * @param[in] g the graph involved
 * @param[in] n the node to add inside the graph
 */
void addNodeInGraph(PredSuccGraph* g, const Node* n);

/**
 * Adds a node within the graph
 *
 * @param[in] g the graph to involved
 * @param[in] id the id of the node to create
 * @param[in] paylaod the paylaod associated to the node
 * @return the node you've just created
 */
Node* addNewNodeInGraph(PredSuccGraph* g, NodeId id, void* payload);

/**
 * Get the node in the graph
 *
 * @param[in] g the graph involved
 * @param[in] id the unique identifier of the node you want to fetch
 * @return
 * 	\li the ::Node instance;
 * 	\li NULL if there is no such node
 */
Node* getNodeInGraph(const PredSuccGraph* g, const NodeId id);

/**
 * Get the number of successors of the given node in the graph
 *
 * @param[in] g the graph to analyze
 * @param[in] id the id of the node to consider
 * @return the number of successors of the node with id \c id inside the graph \c g
 */
int getNumberOfEdgesOfNodeInGraph(const PredSuccGraph* g, NodeId id);

/**
 * Adds an edge between 2 nodes
 *
 * \dotfile edge01.dot
 *
 * @param[in] the graph to alter
 * @param[in] the edge to add inside the graph.
 * @return \c edge itself
 */
Edge* addEdgeDirectlyInGraph(PredSuccGraph* g, const Edge* edge);

/**
 * Adds an edge between 2 nodes
 *
 * \dotfile edge01.dot
 *
 * @param[in] source the source node
 * @param[in] sink the sinnk node
 * @param[in] edgePayload the label on the edge we need to add
 * @return the edge just added
 */
Edge* addEdgeInGraph(Node* restrict source, Node* restrict sink, void* edgePayload);

/**
 * Adds an edge between 2 nodes
 *
 * \dotfile edge01.dot
 *
 * @param[inout] graph the graph to alter
 * @param[in] sourceId the id of the source node
 * @param[in] sinkId the id of the sink node
 * @param[in] edgePayload the label on the edge we need to add
 * @return the edge just added
 */
Edge* addEdgeInGraphById(PredSuccGraph* graph, NodeId sourceId, NodeId sinkId, void* edgePayload);

/**
 * Removes an edge between 2 nodes
 *
 * \dotfile edge01.dot
 *
 * The function does nothing if there is not edge between \c source and \c sink
 *
 * \post
 * 	\li the edge between the 2 will be removed (even in the memory);
 *
 * @param[in] source the source node
 * @param[in] sink the sink node
 */
void removeEdgeInGraph(Node* restrict source, Node* restrict sink);

/**
 * Like ::removeEdgeInGraph but operates on ids, not on the concrete node instances.
 *
 * Useful if 2 different graphs shares the same layout
 *
 * \post
 * 	\li \c graph will not contain the edge "sink -> source";
 * 	\li if the edge was not present in the first place, nothing changes: the function does nothing;
 *
 * \note
 * The edge is removed from the memory as well;
 *
 * @param[inout] graph where we need to remove an edge;
 * @param[in] sourceId the id of the source of the edge;
 * @param[in] sinkId the id of the sink of the edge;
 * @param[in] removeFlipped true if tyou want to remove not only source->sink but also sink->source
 */
void removeEdgeInGraphById(PredSuccGraph* graph, const NodeId sourceId, const NodeId sinkId, const bool removeFlipped);

/**
 *
 * Removes an edge from the graph
 *
 * The function won't delete the edge from memory however
 *
 * \dotfile edge01.dot
 *
 * @param[in] source the source of the node
 * @param[in] sink the sink of the node
 * @return
 * 	\li the edge between the 2;
 * 	\li NULL if there is no edge between the 2
 */
Edge* getEdgeInGraph(const Node* restrict source, const Node* restrict sink);

/**
 * Like ::getEdgeInGraph, but uses ::Node id instead of ::Node intances
 *
 * @param[in] graph the graph involved
 * @param[in] sourceId the id of the source node
 * @param[in] sinkId the id of the sink node
 * @return
 * 	\li the edge between the 2;
 * 	\li NULL if  there is no edge between the 2
 */
Edge* getEdgeInGraphById(const PredSuccGraph* graph, const NodeId sourceId, const NodeId sinkId);

/**
 * @param[in] graph the graph to check
 * @param[in] sourceId the id of the source of the edge to check
 * @param[in] sinkid the id of the sink of the edge to check
 * @return
 * 	\li true if there is an edge going from \c sourceId to \c sinkid
 * 	\li false otherwise
 */
bool containsEdgeInGraphById(const PredSuccGraph* graph, NodeId sourceId, NodeId sinkId);

/**
 * like ::computeDotFile but generate a png directly
 *
 * The function creates a temporary "*.dot" file, but such file is also automatically deleted.
 *
 * \attention
 * this function required "circo" to be installed on the system. Please install it
 * via <tt>sudo apt-get install graphviz</tt>
 *
 * \post
 * 	\li in the CWD a .png file is present;
 *
 * @param[in] graph the graph involved
 * @param[in] fileName the filename (with no extensions) to create
 */
void printGraph(PredSuccGraph* graph, const char* fileName);

/**
 * Like ::printGraph but it accept an additional string
 *
 * Useful for debugging purposes
 *
 * @param[in] graph the graph involved
 * @param[in] fileName the filename (with no extensions) to create
 */
void printGraphWithAdditionalName(PredSuccGraph* graph, const char* fileName, const char* additionalName);

/**
 * Prints a graph with the most generic possible prototype
 *
 * \attention
 * this function requires GraphViz installed in order to work". Please install it
 * via <tt>sudo apt-get install graphviz</tt>
 *
 * \post
 * 	\li in the CWD a .png file is present;
 *
 * @param[in] graph the graph involved
 * @param[in] cmd the "dot" command to use. If in doubt, use ::DC_DOT
 * @param[in] fileName the filename (with no extensions) to create
 * @param[in] additionalName another name to add to the filename of the PNG to create
 * @param[in] highlightedNode the node to highlight in the graph
 */
void printGraphWithOptions(const PredSuccGraph* graph, dot_commands cmd, const char* fileName, const char* additionalName, NodeId highlightedNode);

/**
 * @param[in] graph the graph involved
 * @return the number of nodes inside the graph
 */
int getSizeOfGraph(const PredSuccGraph* graph);

/**
 * Creates a list of all the edges in the graph
 *
 * The list created contains the actual edges, so be sure to know what you're doing if you alter them
 *
 * \attention
 * The function \b does create the list in the heap, but it doesn't explicitly free it. Se be sure to call
 * ::destroyForwardList to free the list from the memory
 *
 * @param[in] graph the graph to analyze
 * @return the list fo edges in the graph
 */
EdgeList* getEdgeListOfPredSuccGraph(const PredSuccGraph* graph);

/**
 * If you want to fetch the edges themselves, and not just the number, see ::getEdgeListOfPredSuccGraph
 *
 * @param[in] the graph to analyze
 * @return the number of edges inside the graph
 */
int getEdgesSize(const PredSuccGraph* graph);

/**
 * \attention
 * This operation **heavily** depends on graph size
 *
 * @param[in] g1 graph involved
 * @param[in] g2 graph involved
 * @return true if \f$ g1 = g2 \f$, false otherwise
 */
bool predSuccGraphCmp(const PredSuccGraph* restrict g1, const PredSuccGraph* restrict g2);

/**
 * Save a whole graph inside a file
 *
 * \pre
 * 	\li \c f open for writing binary
 *
 * \note
 *  This function **won't** store the function callbacks. This because function cannot be serialized!
 *
 * @param[inout] f the file to write on.
 * @param[in] g the graph to save
 * @return
 * 	\li true if the operation is successful;
 * 	\li false otherwise;
 */
void storeGraphInFile(FILE* f, const PredSuccGraph* g);

/**
 * Load a graph from a file
 *
 * \pre
 * 	\li \c f open for reading binary
 *
 * \note
 *  This function **won't** load the function callbacks. This because function cannot be serialized!
 *
 * @param[inout] f the file to read from
 * @return
 * 	\li the instance of graph read;
 * 	\li NULL if we couldn't read the file at al;
 */
PredSuccGraph* loadGraphFromFile(FILE* f, object_deserializer nodeDeserializer, object_deserializer edgeDeserializer);

#endif /* PREDSUCCGRAPH_H_ */
